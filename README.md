# Snake
This is for the "snake" game, based on Luchang's AI program.

## Structure
`Battle` class contains the game board and two players, it coordinates the moves of each player and displays the board (on real time hopefully).

## TODO
- [] Add `Player` class, which represents the AI program. It should be able to interact with the Luchang's AI bots, using std `fork()` and `pipe()` `fdopen()` `kill()` etc. to communicate. 
- [] Add `Battle` class to coordinate the players' moves and store them.
- [] Using openGL to display the board. How about wrtting a `display` class and use **observer pattern** to interact with `Battle`?
- [] Display the game in realtime
- [] Add mouse and/or keyboard interaction.

## Puzzles
- why I can't call `pipe()` in `Player` constructor while `fork()` in member function?
