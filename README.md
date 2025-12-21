# Chess Engine (SDL2)

## 🎓 Computer Programming I – Final Project

A full chess game implemented in C using SDL2, featuring a graphical interface, sound effects, and core chess rules.
This project was developed as part of the Computer Programming I course.
## 👓 Supervision
- Prof.Dr.Marwan Torki
- Eng. Karim Alaa

## 📌 Features

- Graphical chess board using SDL2

- Piece rendering with SDL2_image

- Text rendering with SDL2_ttf

- Sound effects using SDL2_mixer

- Background music making you enthusiastic

- Legal move validation

- Supporting enpassent , castling , pawnpromotion and stalemate

- The Game supports redo and undo till the first move

- Turn-based gameplay

- Offering five slots to save games and continue them later

- Highlight valid moves squares when piece is dragged

- Highlight king square with red when he is in check position

- Highlight last move squares with yellow

- Show several message boxes during gameplay

- Clean modular C codebase

- Adding an Icon for the executable game

- Cross-platform support (Windows & Linux)

## 👥 Team Members

- Youssef Essam ElDeen Mahmoud ElSaeed (Id : 24010854)

- Abdelwahhab Khaled Khamis (Id : )

## 🛠️ Technologies Used

- Language: C

- Libraries:

>SDL2

>SDL2_ttf

>SDL2_image

>SDL2_mixer

>Build Tool: Makefile

## 📂 Project Structure (Simplified)

```text
Chess-Master-CSED28/
├── src/            # Source files
├── cfg/            # For binary saved games
├── include/        # Header files
├── assets/         # Images, fonts, sounds
├── thirdparty/     # SDL libraries (Windows only)
├── Makefile        # Build configuration
└── README.md       # Project documentation
```
---

##  📈 Overview

The SDL2 Chess Engine is a high-performance graphical board game application implemented in C. It evolves the complexity of traditional chess logic into a modern, interactive experience by bridging a robust backend engine with a hardware-accelerated frontend. The application strictly adheres to FIDE regulations, accurately handling technical maneuvers such as kingside/queenside castling, en passant captures, and pawn promotion.

---
## 🚀 Build & Run Instructions

### 🔹 Windows

#### Requirements

- **GCC (MinGW)**
- **SDL libraries**

| Library        | Version | Link |
|---------------|---------|------|
| SDL2          | 2.24.0  | https://github.com/libsdl-org/SDL/releases |
| SDL2_ttf      | 2.24.0  | https://github.com/libsdl-org/SDL_ttf/releases |
| SDL2_image    | 2.7.1   | https://github.com/libsdl-org/SDL_image/releases |
| SDL2_mixer    | 2.8.1   | https://github.com/libsdl-org/SDL_mixer/releases |

---

#### Setup

```bash
git clone https://github.com/Joessameldinq/Chess-Master-CSED28-.git
cd Chess-Master-CSED28-
```
- Create a folder named thirdparty

- Copy the following folders into it:

- SDL2-2.24.0

- SDL2_ttf-2.24.0

- SDL2_image-2.7.1

- SDL2_mixer-2.8.1

- Copy all *.dll files from the SDL bin directories into the folder containing Chess.exe

```bash
make clean run
```

### 🔹 Linux(Ubuntu)
#### Requirements

 - GCC: The GNU C Compiler.
 - pkg-config: A critical tool that helps your compiler find the correct paths for the SDL2 headers and libraries.

##### 1. Core Development Tools

###### Install Commands

 ```bash
sudo apt update
sudo apt install build-essential pkg-config
```

##### 2. SDL2 Development Libraries

###### Requirements for this Project

- Base Library: libsdl2-dev
- libsdl2-image-dev (PNG, JPG support)
- libsdl2-ttf-dev (TrueType Font support)
- libsdl2-mixer-dev (Audio/Music support)

###### Install Commands 

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

##### 3. Compilation and run

```bash
gcc src/.*c -o Chess `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf
./Chess
```


##  🧠 Overview of the Design  

### 🛠 Moving Logic 

#### Piece Moving-Validation Functions 

> **Note** most of project variables and functions named are in the camelCase convention

- We used function for each piece to check the validity of piece move
- In most functions we used difference and absolute difference between rows and columns instead of using direction arrays 

| Function prototype | Functionality |
| --- | --- | 
| `bool isValidPawn(Game* game,Piece piece,Move move)`| **Checks single/double steps forward, diagonal capture, and En Passant logic based on the enPassentAvailable flag.**|
| `bool isValidRook(Game* game,Piece piece,Move move)`| **Validates horizontal and vertical movement**|
| `bool isValidBishop(Game* game,Piece piece,Move move)`| **Validates diagonal movement by checking if the absolute row difference equals the column difference.**|
| `bool isValidKnight(Game* game,Piece piece,Move move)`| **Checks for the unique ""L-shape"" (2×1 or 1×2 squares)**|
| `bool isValidQueen(Game* game,Piece piece,Move move)`| **Combines the logic of both isValidRook and isValidBishop to allow multi-directional sliding.**|
| `bool isValidKing(Game* game,Piece piece,Move move)`| **Handles standard 1-square movement in any direction and coordinates with isValidCastling**|
| `bool isValidCastling(Game* game,Piece piece,Move move)`| **A specialized check for King/Rook safety, verifying that neither have moved and squares are not under attack.**|

#### Utilities for Moving-Validation
| Function prototype | Functionality |
| --- | --- | 
| `int findFirstEmptyCapturedSlot(Piece arr[])`| **Finds the number of Captured Pieces of either player**|
| `bool canPieceMoveTo(Game *game, Piece piece, Move move)`| **Check if piece can move to pos only geometrically we use it in the function isSquareAttacked which is used to determine check**|
| `bool isPathClear(Game *game, Move move)`| **Check whether every square between initial (exclusive) and final (exclusive) is empty. Works for straight and diagonal directions using unit step.**|
| `bool simulateMoveAndShowIfInCheck(Game game, Move move)`| **One of them most important function it's considered the last level before applying the move it make a backup of the current game state and Check if the move will keep the king in check or not. if it make the king incheck the move is invalid and we ask the user to input another move(in the GUI part there is a beeb sound effect That works generally for invalid moves)**|
| `bool isLegalMove(Game *game, Piece piece, Move move)`| **piece-specific movement rules from square to another (does NOT check king safety) and is regarded as large switch replacement.**|
| `bool isValidMove(Game *game, Move move)`| **The last step before making move.But we shouldn't use it in simulate move as it will lead to infinite recursion as simulatemove is part of isValidMove.So in simulate moves we manually apply move -> check if king isInCheck -> redo the move and game state**|
| `bool isSquareAttacked(Game *game, Position pos)`| **Returns true if a position is attacked by the opponent of currentPlayer.It ignores king safety for the moved piece.**|





The engine utilizes a tiered validation system to handle the distinction between "geometric capability" and "legal moves" under FIDE rules. This separation is critical to prevent infinite recursion during check detection.It appears for these functions to be redundant but see this difference between them.
- 1. canPieceMoveTo() vs. isValidMove()


|Function|	Type|	Responsibility|
| --- | --- | ---|
|'canPieceMoveTo'|**Internal / Geometric**|**Determines if a piece can physically reach a square based on its move pattern and path obstructions. It ignores King safety.**|
|'isValidMove'|**External / Legal**|**The top-level validator. It checks turn order, board boundaries, and performs move simulation to ensure the King is not left in check.**|
- 2. Preventing Infinite Recursion

A common pitfall in chess programming is a circular dependency: isValidMove checks for Check, which calls isSquareAttacked, which would normally call isValidMove.

To solve this, our engine breaks the loop:

    isValidMove simulates a move and calls inCheck().

    inCheck() calls isSquareAttacked().

    isSquareAttacked() calls the lightweight canPieceMoveTo(), which does not look for checks, thus terminating the recursive chain.

3. The Move Simulation Pattern

To ensure the King's safety, the engine employs a "Simulate-Verify-Restore" pattern within isValidMove:

    Simulation: The game state is copied (passed by value) to a temporary Game struct.

    Verification: applyMove() is executed on the copy, and inCheck() is queried.

    Result: If the simulation reveals the King is under attack, the move is flagged as illegal and rejected before the actual board state is ever modified.

And after passing all of these validations the last part is applying the move

|Function Prototype| Functionality|
|---|---|
|void applyMove(Game *game, Move *move)|**This function doesn't check if the movid valid or not.It just applies the move.So isValidMove should be called and if it returns true we apply the move.**|

- This function change the move structure enumerators (MoveType and capturedPiece) and change the whole game state like board pieces , captured pieces and current player turn. 
- In this function we don't compute the gamestate as it will lead to infinite recursion. Caller in game loop will invoke computeGameState function. Since computeGameState() must check for legal moves to determine Stalemate/Checkmate, calling it inside the move-application logic would create a recursive loop.
- The main game loop serves as the orchestrator: it calls isValidMove, then applyMove, and finally computeGameState as three distinct, sequential steps.

After we finally apply the move we set the game flags to false again for next turns (We Call it at the beginning of applyMove but it won't matter calling it at the beginning or at the end).
|Function|
| --- |
|'void setFlagsFalse(Game *game)'|

#### Summary for Moving Logic
Once a move passes the multi-tiered validation (Geometry → Piece Rules → King Safety), the engine transitions from Validation to Execution.

|Phase|	Responsibility|	Key Function|
| --- | --- | ---|
|Execution|	Modifies the board, handles special moves (Castling/En Passant), updates the 50-move clock, and swaps player turns.|	**applyMove()**|
|Cleanup|Resets temporary flags (like en passant availability) to ensure the next turn starts with a clean state|**setFlagsFalse()**|
|Evaluation|Analyzes the new board state to check for Checkmate, Stalemate, or Insufficient Material.|**computeGameState()**|




###  🚧🏴 Ending Conditions

### Basic Functions

|**Function Prototype**| **Functionality**|
| ---                  |    ---           |
|`bool inCheck(Game *game)`|**Checks if the current player's king is in check(game->currentPlayer) using isSquareAttacked().It must answer ONE question only: Is the king's square currently attacked by any enemy piece? Only geometrically It doesn't matter whether the attacker leave the other king in check or not**|
|`bool inCheckMate(Game *game)`|**Checks if the current player's king is in checkmate (in check and can't escape from this position). It tries every possible move for the current player and exits early if at least one move is legal and doesn't keep the king in Check**|
|`bool isStalemate(Game *game)`|**This is one of the possible drawing conditions used in this project. It first check if the king of current player in check if true. it exits early else it tries all possible moves like the previous function (isStalemate) if it finds a legal move it returns false else returns true and game ends by stalemate**|
|`bool fiftyMovesRule(Game *game)`|**if morethan 100 move is made without moving pawns the game ends by fifty-move-rule (draw) following the FIDE rules**|
|`bool isDeadPosition(Game *game)`|**Check for suffecient pieces for the player to win if both doesnt have that suffecient pieces we declare a draw.**|


### Utilities

|**Function Prototype**| **Functionality**|
| ---                  |    ---           |
|`Color getSquareColor(int row,int col)`|**It's helpful tool in the dead position function as we calculate things like the number of bishop pieces on black squares**|
|`Position findKingPosition(Game *game, Color color) `|**It finds the king belonging to a given color**|
|`GameStatus computeGameStatus(Game *game)`|**After applying move this function update the game state**|

🏴‍☠️ The "Dead Position" Limitation

While the isDeadPosition function accurately handles material-based draws with 99% of sucess, it is important to note its technical constraints:

    Material Accuracy: The function perfectly identifies draws for King vs. King, King & Bishop vs. King, King & Knight vs. King, and King & Bishops of the same color vs. King & Bishop.

    Geometric Deadlocks: The current implementation does not detect "Blocked Positions" (e.g., pawn chains that completely seal the board). In such cases, the engine relies on the Fifty-Move Rule or Stalemate logic to eventually terminate the game.

    Helpmate Complexity: Following FIDE standards, scenarios like King & Knight vs. King & Knight are not flagged as dead positions because a checkmate is technically possible if a player blunders their King into a corner blocked by their own piece.

    Most chess sites (like Chess.com or Lichess) use a mix of "Material-based" rules and the "50-move rule" to catch the weird blockaded positions. If a position is truly dead but doesn't match a material rule, the players will eventually hit the 50-move limit anyway.