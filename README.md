# Chess Engine (SDL2)

## 🎓 Computer Programming I – Final Project

A full chess game implemented in C using SDL2, featuring a graphical interface, sound effects, and core chess rules.
This project was developed as part of the Computer Programming I course.

## ⚔️🏇🏼 Knight's Tour Puzzle

🎥 Click to watch the Knight's Tour Demo
<p align="center">
  <video src="https://github.com/user-attachments/assets/b799ac29-8eef-4d5f-8e2c-a249c1810120" width="800" controls>
  </video>
</p>



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

- Offering eight slots to save games and continue them later

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

## 🎯User Manual
### ♟️ Chess Master: User Manual

Welcome to the SDL2 Chess Engine. This application provides a full-featured chess experience with a graphical interface, sound effects, and strict adherence to FIDE rules.
### 🎮 Getting Started
1. Launching the Game

    Main Menu: Upon launching, you will see the Main Menu. You have three primary options:

        New Game: Starts a fresh match between White and Black.

        Load Game: Opens a menu to resume a previously saved match.

        Quit: Safely exits the application.

2. Basic Controls

    Selecting a Piece: Click and hold the Left Mouse Button on the piece you wish to move.

    Moving: Drag the piece to your target square.

    Visual Aids: * When you drag a piece, valid destination squares will be highlighted in green.

        The last move made is highlighted in yellow for both the starting and ending squares.

        If your King is in check, the square beneath it will turn red.

## 🛠️ In-Game Features

Once a match starts, several control buttons are available at the bottom of the screen:
Button	Action
Undo	Reverts the board to the previous turn. You can undo all the way back to the start.
Redo	Reapplies a move that was just undone.
Save	Opens 8 save slots. Choose one to record your current game state.
Back	Returns you to the Main Menu (ensure you save first!).
Draw	Sends a draw offer to the opponent.
## 🔊 Sound Effects

The game provides audio feedback for various actions. You will hear distinct sounds for:

    Standard moves and captures.

    Special moves (Castling, En Passant, Pawn Promotion).

    Game alerts (Check, Checkmate, or Stalemate).

    Invalid Moves: If you try to move a piece illegally, a "beep" will play, and the piece will return to its original spot.

## 📜 Special Chess Rules Supported

- The engine automatically handles complex chess maneuvers:

    - **Castling**: If neither your King nor Rook has moved and the path is clear/safe, you can move the King two squares toward the Rook to castle.

    - **En Passant**: A special pawn capture that can only occur immediately after an opponent moves a pawn two squares forward.

    - **Pawn Promotion**: When a pawn reaches the 8th rank, a menu will allow you to choose a Queen, Rook, Bishop, or Knight.

    - **Draw Rules**: The engine detects Stalemate, Insufficient Material (Dead Position), and the 50-Move Rule.

## 💾 Saving and Loading

- **To Save** : Click the Save button. A message box will prompt you to select a slot (1–8). This saves the exact board state, turn order, and move history to the /cfg folder.

- **To Load**: From the Main Menu, click Load Game. Select the slot you used previously. The game will restore the board exactly as you left it.

        Note: The Undo/Redo history is not saved between sessions.

## 🛠️ Troubleshooting & Tips

- Window Responsiveness: The game uses SDL_WaitEvent to save CPU power. If the window seems "frozen," it is simply waiting for your next mouse click or move.

- Invalid Moves: If a piece won't stay on a square, ensure you aren't leaving your King in check. The engine prevents any move that results in your King being captured.

- Make sure that all assets are in the assets directory

- Make sure that all *.dlls are in the directory of executable file (Dynamic Libraries) if they don't exist a message like this will be shown 
```bash
The code execution cannot proceed because SDL2.dll was not found
```

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
gcc ./src/*.c -o Chess `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf
./Chess
```


##  🧠 Overview of the Design 

### Basic Data Structures Used

- A linked list for stack. It's used in undo/redo process.

```bash
typedef struct Node
{
    Game curGame;
    struct Node* nextGame;
}Node;
```

- Enums for **PieceType** , **PieceColor** ,**GameStauts** and **MoveType**. They make the Code reproducable and maintainable.
```bash
typedef enum
{
    EMPTY = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6
} PieceType;

//Piece colors
typedef enum
{
    NONE = 0,
    WHITE = 1,
    BLACK = 2
} Color;

//Game state
typedef enum
{
    PLAYING,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW_FIFTY_MOVE,
    DRAW_INSUFFICIENT_MATERIAL,
    DRAW_AGREEMENT
} GameStatus;

//Movement type
typedef enum
{
    NORMAL_MOVE,
    CAPTURE,
    CASTLE_KINGSIDE,
    CASTLE_QUEENSIDE,
    EN_PASSENT,
    PAWN_PROMOTION,
    CAPTURE_AND_PAWN_PROMOTION
} MoveType;
```

- Helpers in move process
   - Position instead of passing to functions fromRow,fromColumn,toRow,toColumn we pass the positions of to/from squares.
```bash
typedef struct 
{
    int x; 
    int y; 
} Position;
```

  - Move Struct that holds all needed data for the move 
```bash
typedef struct 
{
    Position initial;            
    Position final;              
    Piece capturedPiece;       
    MoveType moveType;         
    PieceType promotionPiece;  
} Move;
```

- Piece struct

```bash
typedef struct 
{
    PieceType type;     
    Color color;         
    bool hasMoved;       
} Piece;
```
- Flags for special moves

```bash
typedef struct
{
    bool pawnPromotionMade;  
    bool castlingMade;       
    bool enpassentMade;     
} GameFlags;
```
- Basic Game struct 

```bash
typedef struct 
{
    Piece board[BOARD_SIZE][BOARD_SIZE];    
    Piece capturedWhitePieces[16];         
    Piece capturedBlackPieces[16];         
    int halfMoveClock;                    
    GameStatus status;                    
    Color currentPlayer;                  
    Position enPassentTarget;               
    GameFlags currentFlag;
    bool enPassentAvailable;                 
} Game;
```





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

🏴‍☠️ ⚠️The "Dead Position" Limitation

While the isDeadPosition function accurately handles material-based draws with 99% of sucess, it is important to note its technical constraints:

    Material Accuracy: The function perfectly identifies draws for King vs. King, King & Bishop vs. King, King & Knight vs. King, and King & Bishops of the same color vs. King & Bishop.

    Geometric Deadlocks: The current implementation does not detect "Blocked Positions" (e.g., pawn chains that completely seal the board). In such cases, the engine relies on the Fifty-Move Rule or Stalemate logic to eventually terminate the game.

    Helpmate Complexity: Following FIDE standards, scenarios like King & Knight vs. King & Knight are not flagged as dead positions because a checkmate is technically possible if a player blunders their King into a corner blocked by their own piece.

    Most chess sites (like Chess.com or Lichess) use a mix of "Material-based" rules and the "50-move rule" to catch the weird blockaded positions. If a position is truly dead but doesn't match a material rule, the players will eventually hit the 50-move limit anyway.

### ♾️ Saving and Loading Games

 - This engine save and load games in binary format 
 - The architecutre of this project making the whole game snapshot in a struct allow us to save the binary format of this struct in a file and load it and continue the game at any time.
 - We don't save the undo stack.
 - Many helpers is used in SavingLoading.c so we used static functions.By default, every function in C is "global." This means if you define a function in file_a.c, you can call it from file_b.c. A static function, however, is restricted to the file in which it is defined and this is convenient to our case as we won't use this helpers outside the file scope.

#### Utilities

These helper functions are used to read and write integers,booleans and enumerators from/to the file.

 - `static bool writeInt(FILE *fp, int v)` 
 - `static bool writeBool(FILE *fp, bool v)` 
 - `static bool writeEnum(FILE *fp, int v)` 
 - `static bool readInt(FILE *fp, int v)` 
 - `static bool readBool(FILE *fp, bool v)` 
 - `static bool readEnum(FILE *fp, int v)` 

### Basic Functions

- `bool saveGame(FILE *fp, const Game *g)` 
    - it takes a pointer to file and constant pointer to game(to avoid changing the game state) as input.
    - it writes  magic header and version number in the beginning to allow us validate the file when loading the game.
    - it writes all game struct contents in the binary file in a specific order that we must apply the same order when loading game.
    - This function validate each write in the file.
    - it returns true if the saving game process succeeded.
- bool `loadGame(FILE *fp, Game *g)`
- it takes a pointer to file and a pointer to game(to load the contents in it) as input.
    - it reades  magic header and version number in the beginning to ensure that the file contents are valid.
    - it reads all game struct contents in the binary file in the same order we applied when saving the game (order matters).
    - This function validate each read from the file.
    - it returns true if the loading game process succeeded.

- The main game loop will open the file and take the slot as input(messge box)to save/load game to/from the cfg directory.
- This game provides eight slots for saving and loadig but more and more slots can be made.


### ⚙️ Undo and redo 

- In this part we use stacks to keep tracking game snapshots.
- If we need to undo a game a game turn is popped from game stack to redo stack
- If we need to redo a game a game turn is popped from redo stack to game stack

#### Stack Helpers

- `bool push(Node **stack,Game game)` 
    - To add a game to the top of a stack
- `Game pop(Node **stack)`
    - To remove a game from the top of a stack
- `Node* initializeStack()`
    - To create a new empty stack
- `bool isEmptyStack(Node *stack)`
- `void clearStack(Node **stack)`


### Basic Function in this process

|Function Prototype| Functionality|
| --- | --- |
| `bool undoMove(Node **gameStack, Node **redoStack)`| **Check first if there is at least two states to undo. It pop a game state from the game stack to the redo stack**|
| `bool redoMove(Node **gameStack, Node **redoStack)`| **Check first if the redo stack isn't empty. It pop a game state from the redo stack and push it to the game stack**|


## 🎉 Graphical User Interface

- We have two parts in the gui : main menu and the game mode
- In main menu player can play a new game , load a saved game or quit the game
- If any button is clicked , the button is highlighted with ocean blue color
- Clicking on the load button show a message box for the user to choose a saved game out of eight games


### Game mode
- There are five buttons down the board
    - **Undo Button**
    - **Redo Button**
    - **Save Button**
    - **Back Button**
    - **Draw Agreement Button** 
- Captured pieces are shown right the board in two columns for each player
- The game mode support showing current turn , last move and half move clock.
- It also supports sound effects for moving pieces, capturing , enpassent , castling , pawn promotion , mate , check and stalemate
- A screen is shown after game end show the final result (white wins , black wins or stalemate).
- Threatened king (king in check) is highlighted in red.
- If king in check , a message box appears in the screen.
- Last move squares (from and to squares) are highlighted in yellow.
- If piece is dragged , valid moves are highlighted in green
- If a player tries to make invalid move , a sound effect is played and dragged piece return to it's previous square

### Basic Structures in GUI

- **Button Struct** and its members are a **texture** and a **rectangle**.
```bash
typedef struct 
{
    SDL_Texture *texture;
    SDL_Rect rect;
}Button;
```

- **App Struct** and it holds all data related to the whole app. Its members are a pointer to **window** , **renderer** , a flag indicating game-running **running** , the **gamestate** , a synced **game** , a **gamestack** , a **redostack** and dynamic dimensions for window.
```bash
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    GameState currentScreen;
    Game game;          
    Node *gamestack;    
    Node *redostack;
    // Dynamic dimensions for window resize support
    int windowWidth;
    int windowHeight;
    int boardOffset;
    int squareSize;
    int buttonWidth;
    int buttonHeight;
    int buttonSpacing;
} App;
```

- a gamestate enumerate
```bash
typedef enum {
    SCREEN_MENU,
    SCREEN_GAME,
    SCREEN_QUIT
} GameState;
```

- The game mode ui struct
   - it holds all **buttons** , **board pieces** , **captured pieces** , **texutres for turn indicator and last move** , **soundeffects** and **dynamic dimensions for window** etc.

```bash
typedef struct 
{
    SoundEffects sEffect;
    Button save;
    Button undo;
    Button redo;
    Button back;
    Button drawAgreement;
    Button capWhite;
    Button capBlack;
    Button currentTurn;           
    SDL_Texture *whiteTurnTex;    
    SDL_Texture *blackTurnTex;    
    Button capturedPieces[2][16];
    Button boardPieces[8][8];
    Button lastMove;
    SDL_Texture *fileLabelTextures[8];  // A.png through H.png
    SDL_Texture *rankLabelTextures[8];  // 1.png through 8.png
    SDL_Texture *background;
    SDL_Rect fromMovingRect;
    SDL_Rect toMovingRect;
    SDL_Rect kingThreaten;
    TTF_Font *moveFont;  // Font for displaying move notation (loaded once at init)
    // Dynamic dimensions for resize support
    int squareSize;
    int boardOffset;
    int windowWidth;
    int windowHeight;
} gamegui;
```

- The sound effects enum
```bash
typedef struct
{
    Mix_Chunk * normalMove;
    Mix_Chunk * promotion;
    Mix_Chunk * castling;
    Mix_Chunk * enpassent;
    Mix_Chunk * capture;
    Mix_Chunk *illegalMove;
    Mix_Chunk * checkmate;
    Mix_Chunk * check;
    Mix_Chunk * stalemate;
    Mix_Chunk * draw;
    Mix_Chunk * drawOffer;
}SoundEffects;
```


- Main menu struct

```bash
typedef struct 
{
    SDL_Texture *background;
    Button newgame;
    Button loadgame;
    Button quit;
    
}mainmenugui;
```
### Common Functions used in GUI

 - **Note** All loading and destroying (fread) processes are save using check NULLITY. So, No overflow occur and no un-fread memory locations. 
#### Created Helpers
|**Function**   |     **Usage** |
| --- | -- |
|`SDL_Texture *loadtexture(const char *file, SDL_Renderer *renderer)`| **Create texture from a loaded image. it takes the file name and the renderer as input and output a texture**|
|`bool isButtonClicked(int mx,int my,Button button)`|**One of the most important helpers as it checks the event of clicking button.It checks wheater the mouse coordinates in the boundaries of button rectangle.**|
|`void highlightClickedButton(SDL_Renderer *renderer,Button cButton)`|**It highlights the clicked button with blue ocean transparent color**|
|`void playSoundEffect(Mix_Chunk *sEffect)`|**This function is used many times in the main game loop to play sound effects when a check , checkmate , stalemate or any special move happens.It takes a chunk as input to play it**|.


- **Generally This is the template used to display main menu and game mode**
  - WHILE(RUNNING)
       - RENDER OBJECTS
       - HANDLE EVENTS
       - UPDATE GUI

- **Basic Functions** in the main menu part
    - `initMainMenuGui()` **it returns a pointer to mainmenugui struct (initialized with all needed textures and rectangles)**.
    - `renderMainMenu()` 
    - `Menu_HandleEvent()` **Handle loading , playing a new game or quit events** .
    - `runMainMenuGui()`  **The function called in the app loop.**
    - `destroyMainMenuGui()` **It destroys all loaded textures with safety check to avoid freeing null pointers or double free**.

- and the same architecture is followed in the game mode gui.

- **NOTE**
- The clicks are controlled by the SDL_WaitEvent so that the game waits until there an input and then start to handle it as a result the CPU usage is low. And here is a comparison between wait event and poll event.
- SDL_WaitEvent and SDL_PollEvent are both functions used to retrieve events from the SDL event queue, but they differ in how they handle an empty queue: SDL_WaitEvent blocks (waits) for an event, while SDL_PollEvent returns immediately. 
- **SDL_PollEvent()**

    Behavior: Checks the event queue. If an event is present, it removes it, fills the SDL_Event structure, and returns 1. If the queue is empty, it returns 0 immediately without waiting.
    Use Case: This is typically used in the main loop of a real-time application or game. Because it doesn't wait, the rest of the game loop (game logic updates, rendering, etc.) can continue to run every frame, even if there is no user input. The application remains responsive and continues to render.
    Performance: Calling SDL_PollEvent in a tight loop constantly uses CPU cycles, which can be inefficient if the application has nothing else to do that frame. 

- **SDL_WaitEvent()**

    Behavior: This function blocks the program and will not return until an event has been added to the queue.
    Use Case: It is ideal for applications that are primarily event-driven and don't need continuous rendering (e.g., a simple menu-based application or a utility program). It allows the operating system to put the CPU into a lower power mode when idle, which is very important for battery life on mobile devices.
    Performance: It is very CPU efficient because it yields control back to the operating system, but it can make an application seem sluggish if not used properly in a game loop, as the game won't update or render until a new event occurs.


### ®️ Refrences

- The hirerchy of the project and splitting the project and *.c & *.h files was inspired by 
**K.N.King , C Programming, A Modern Approach, Second Edition Chapter 10:** **Program Organization** 

- Makefile
    - **https://makefiletutorial.com/**
    - **-DSDL_MAIN_HANDLED flag** **https://stackoverflow.com/questions/60948791/sdl2-cmake-undefined-reference-to-winmain16-on-win10-mingw**

- clearScreen() in console part 
    - **https://www.geeksforgeeks.org/c/clear-console-c-language/**

- Using Static functions in saving and loading processes
    - Storage Class of a function section in **K.N.King Chapter 18: Declarations**

- Saving and loading using binary format
    - **K.N.King Chapter 22: Input/Output 22.6 Block I/O**

- Stack data structure
    - **K.N.King Chapter 12: Pointers and Arrays from p262** 
    - **https://www.geeksforgeeks.org/dsa/stack-data-structure/**

- `<ctype.h>` functions like tolower()
    - **https://cplusplus.com/reference/cctype/**

- Compiling Resource file *.rc (for the icon of executable file)
    - **https://stackoverflow.com/questions/708238/how-do-i-add-an-icon-to-a-mingw-gcc-compiled-executable**
- Preprocessor directives
    - **https://www.geeksforgeeks.org/cpp/cpp-preprocessor-directives-set-2/**
- All SDL stuff 
    - **SDL_Wiki **https://wiki.libsdl.org/SDL2/FrontPage** 
    - **Lazy foo **https://lazyfoo.net/tutorials/SDL/**
    - **http://gamedevgeek.com/tutorials/getting-started-with-sdl/**
    - **https://www.studyplan.dev/sdl2/sdl-setup-windows#setting-up-sdl2-in-windows-visual-studio** (Fixed my bug when *.dlls weren't in the .exe directory)

    






  

 


