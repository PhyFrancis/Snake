# Snake
This is for the "snake" game, based on Luchang's AI program.

## Structure
`Battle` class contains the game board and two players, it coordinates the moves of each player and displays the board (on real time hopefully).

## TODO
- [X] Add `Player` class, which represents the AI program. It should be able to interact with the Luchang's AI bots, using std `fork()` and `pipe()` `fdopen()` `kill()` etc. to communicate. 
- [X] Add `Battle` class to coordinate the players' moves and store them.
- [X] Add `Client Protocol` to define the interface of `Observer` or `Human Player`, it also separates the ugly OpenGL stuff from the main function.
- [] let `Battle` support multiple Clients.
- [] Add mouse and/or keyboard interaction.

## Puzzles
- It seems I can't call `pipe()` in `Player` constructor while `fork()` in member function.
