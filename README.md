# Snake
This is for the "snake" game, based on Luchang's AI program.

## Structure
`Battle` class contains the game board and two players, it coordinates the moves of each player and displays the board (on real time hopefully).

## TODO
- [X] Add `Player` class, which represents the AI program. It should be able to interact with the Luchang's AI bots, using std `fork()` and `pipe()` `fdopen()` `kill()` etc. to communicate. 
- [X] Add `Battle` class to coordinate the players' moves and store them.
- [X] Add `Client Protocol` to define the interface of `Observer` or `Human Player`, it also separates the ugly OpenGL stuff from the main function.
- [X] let `Battle` support multiple Clients.
- [X] Add mouse and/or keyboard interaction for camera.
- [] Add a human player client who can play against computer or against each other.
- [] Add a better user interface for the client (start button, user instructions, etc.).
