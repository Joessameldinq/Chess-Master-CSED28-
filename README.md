## Chess (SDL2)

# 🎓 Computer Programming I – Final Project

A full chess game implemented in C using SDL2, featuring a graphical interface, sound effects, and core chess rules.
This project was developed as part of the Computer Programming I course.

# 📌 Features

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

- Cross-platform support (Windows & Linux)

# 👥 Team Members

- Youssef Essam ElDeen Mahmoud ElSaeed (Id : 24010854)

- Abdelwahhab Khaled Khamis (Id : )

# 🛠️ Technologies Used

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

### Moving Logic 

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








