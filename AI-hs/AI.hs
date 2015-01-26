module Main where

import Control.Arrow
import Control.Monad
import Control.Monad.ST
import Data.Array
import Data.Array.ST
import Data.Array.MArray
import Data.List
import Data.Function
import Data.Maybe
import System.Console.GetOpt
import System.Environment
import System.IO
import System.Random

import System.IO.Unsafe

fi x = fromIntegral x

type PlayerMove = (Int, Int)
type GameMove = (PlayerMove, PlayerMove)

type WorldSize = (Int, Int)
type World = Array (Int,Int) Bool

data Game = Game
    { gameHistory :: [GameMove]
    , gameWorld   :: World }

data Options = Options
    { worldSize :: WorldSize }

defaultOptions :: Options
defaultOptions = Options
    { worldSize = (10, 10) }

optDescrs :: [OptDescr (Options -> Options)]
optDescrs =
    [ Option "x" []
        (ReqArg (\str opts ->
            opts { worldSize = first (const $ read str) $ worldSize opts})
            "sizeX")
        "sizeX of the world"
    , Option "y" []
        (ReqArg (\str opts ->
            opts { worldSize = second (const $ read str) $ worldSize opts})
            "sizeY")
        "sizeY of the world" ]

parseMove :: String -> PlayerMove
parseMove moveStr = (x,y) where
    [x, y] = map read $ words moveStr

showMove :: PlayerMove -> String
showMove (x, y) = show x ++ " " ++ show y

main :: IO ()
main = do
    hSetBuffering stdout LineBuffering
    args <- getArgs
    let (optionGens, _, _) = getOpt Permute optDescrs args
        options = foldr ($) defaultOptions optionGens
    myInitMove <- liftM parseMove $ getLine
    hisInitMove <- liftM parseMove $ getLine
    let game = initialGame (worldSize options) (myInitMove, hisInitMove)
    startGame game

startGame :: Game -> IO ()
startGame game = case getNextMove game of
    Nothing -> putStrLn "die"
    Just myMove -> do
        putStrLn $ showMove myMove
        -- hFlush stdout
        hisMove <- liftM parseMove $ getLine
        if myMove == hisMove
        then putStrLn "die"
        else startGame $ moveGame (myMove, hisMove) game

moveGame :: GameMove -> Game -> Game
moveGame (myMove, hisMove) game@(Game { gameHistory = history, gameWorld = world }) = game
    { gameHistory = (myMove, hisMove):history
    , gameWorld = world // [(myMove, False), (hisMove, False)] }

initialGame :: WorldSize -> GameMove -> Game
initialGame (sizeX, sizeY) initGameMove = game where
    game = moveGame initGameMove $ Game
        { gameHistory = []
        , gameWorld = listArray b
            [ x /= -1 && y /= -1 && x /= sizeX && y /= sizeY | (x,y) <- range b ] }
    b = ((-1,-1), (sizeX, sizeY))

getCurrentMove :: Game -> GameMove
getCurrentMove = head . gameHistory

getMyCurrentMove :: Game -> PlayerMove
getMyCurrentMove = fst . getCurrentMove

getHisCurrentMove :: Game -> PlayerMove
getHisCurrentMove = snd . getCurrentMove

isFree :: Game -> PlayerMove -> Bool
isFree game move = gameWorld game ! move 

nearMoves :: PlayerMove -> [PlayerMove]
nearMoves (x,y) = [(x,y+1), (x+1,y), (x,y-1), (x-1,y)]

possibleNextMove :: Game -> [PlayerMove]
possibleNextMove game = filter (isFree game) . nearMoves . getMyCurrentMove $ game

getNextMove :: Game -> Maybe PlayerMove
getNextMove game = case possibleNextMove game of
    [] -> Nothing
    moves -> findNearest game
    -- moves -> (findNearestSimple game, game { gameSeed = s })

findNearestSimple :: Game -> Maybe PlayerMove
findNearestSimple game@(Game { gameHistory = (_, hisMove):_ , gameWorld = world }) =
    takeBest $ sortBy (compare `on` distanceSimple hisMove) $ possibleNextMove game
  where
    takeBest [] = Nothing
    takeBest [x] = Just x
    takeBest (x:rs) | distanceSimple hisMove x <= 1 = takeBest rs
                    | otherwise = Just x

distanceSimple :: PlayerMove -> PlayerMove -> Int
distanceSimple (x,y) (x',y') = sqr (x'-x) + sqr (y'-y) where
    sqr n = n*n

findNearest :: Game -> Maybe PlayerMove
findNearest game@(Game { gameHistory = (myMove, hisMove):_ , gameWorld = world }) =
    takeBest $ sortBy (compare `on` distanceSimple hisMove) $ possibleNextMove game
  where
    maybePoint = case findPath game hisMove myMove of
        Nothing -> Nothing
        Just path -> Just $ path !! 1
    takeBest [] = Nothing
    takeBest [x] = Just x
    takeBest (x:rs)
        | maybePoint == Nothing = Just x
        | distanceSimple hisMove x <= 4 = takeBest rs
        | otherwise = case maybePoint of
            Nothing -> Just x
            Just point -> if point `elem` (x:rs)
                          then Just point
                          else Just x

findPath :: Game -> PlayerMove -> PlayerMove -> Maybe [PlayerMove]
findPath game@(Game { gameWorld = world }) start end = maybePath where
    maybePath = runST $ do
        state <- initMap
        search state
    traceBack x m = do
        v <- readArray m x
        if fst v == 0
        then return [x]
        else do
            xs <- traceBack (snd v) m
            return (x:xs)
    initMap :: ST s (([PlayerMove],[PlayerMove]), STArray s PlayerMove (Int, PlayerMove))
    initMap = do
        m <- newArray (bounds world) (maxBound,undefined)
        writeArray m start (0,undefined)
        return (([start], []), m)
    search (([], []), m) = return Nothing
    search (([], ys), m) = search ((reverse ys, []), m)
    search (((x:xs), ys), m) = do
        vx <- readArray m x
        let zsOrig = nearMoves x
            zs = (\zs' -> map snd . sort $ zip (cycle $ x:xs) zs') .
                filter (isFree game) $ nearMoves x
        if end `elem` zsOrig
        then do
            writeArray m end (1 + fst vx, x)
            path <- traceBack end m
            return $ Just path
        else do
            zsFinal <- liftM catMaybes $ forM zs (addMove vx x m)
            search ((xs, zsFinal ++ ys), m)
    addMove vx x m z = do
        vz <- readArray m z
        if fst vz <= fst vx + 1
        then return Nothing
        else do
            writeArray m z (1 + fst vx, x)
            return $ Just z
