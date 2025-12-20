## Chess (SDL2)

# 🎓 Computer Programming I – Final Project

A full chess game implemented in C using SDL2, featuring a graphical interface, sound effects, and core chess rules.
This project was developed as part of the Computer Programming I course.

# 📌 Features

- Graphical chess board using SDL2

- Piece rendering with SDL2_image

- Text rendering with SDL2_ttf

Sound effects using SDL2_mixer

- Legal move validation

- Turn-based gameplay

- Clean modular C codebase

- Cross-platform support (Windows & Linux)

# 👥 Team Members

- Youssef Essam ElDeen Mahmoud ElSaeed

- Abdelwahhab Khaled Khamis

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
