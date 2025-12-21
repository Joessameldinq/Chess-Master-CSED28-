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







