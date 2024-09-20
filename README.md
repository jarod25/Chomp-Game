# Chomp Game

Welcome to the **Chomp Game**! This is a two-player strategy game based on the classic game of Chomp, where players take turns "eating" sections of a grid of chocolate squares. The goal is to avoid being forced to eat the poisoned square, which results in a loss.
This project is a group project to learn C Programming language at the ENSISA shcool in Mulhouse, France.

## Table of Contents

- [Game Overview](#game-overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Game Modes](#game-modes)
- [Development](#development)
- [Contributing](#contributing)

## Game Overview

Chomp is a grid-based strategy game where two players alternate turns. On each turn, a player selects a square, and the game removes that square and all squares below and to the right of it. The objective is to avoid selecting the poisoned square in the top-left corner (A1). The player forced to "chomp" that square loses the game.

In this implementation, the grid is set to 7 rows by 9 columns, and each player can chomp up to 5 squares per turn, limiting how many squares can be removed in one move.

## Features

- **Console and GUI Versions**: Play the game in either text-based console mode or using a graphical user interface (GUI) built with GTK 4.
- **AI Opponent**: A smart AI challenges human players by strategically choosing moves that lead towards their victory.
- **Flexible Game Logic**: You can play against the AI, another human, or just watch the AI play against itself.
- **Documentation**: Automatically generated documentation with Doxygen for developers.

## Installation

### Requirements

- **C Compiler** (GCC recommended)
- **GTK 4** (for the graphical version)
- **Doxygen** (optional, for generating documentation)

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/chomp-game.git
   cd chomp-game
   ```

2. Compile the game:
   ```bash
   make
   ```

This will generate the following executables:

- `./build/game`: The console version of the game.
- `./tests/test`: The executable for running unit tests.
- `./docs/docs`: The documentation for the project.

## Usage

### Running Game

To play the game, run the following command:
```bash
./build/game # Options for game mode will be displayed
```

### Running Tests

You can run unit tests using the following command:
```bash
make test # Unless you have already compiled the tests with make
./tests/test
```

### Generating Documentation

If you have Doxygen installed, you can generate the documentation by running:
```bash
make docs # Unless you have already compiled the tests with make
./docs/docs
```

## Game Modes

- **Single Player**: Play against the AI.
- **Two Players**: Play against another human player (console or GUI, local or network).
- **AI vs AI**: Watch the AI play against itself (console or GUI, only with network).

## Development

### Project Structure

- `src/`: Contains source files for the game logic, GUI, and AI.
- `tests/`: Contains source and object files for unit tests.
- `build/`: Directory for storing compiled binaries.
- `docs/`: Directory for generated documentation.
- `include/`: Header files for the project.

## Contributing

- KOHLER Jarod
- RUNSER Lucas
- KOC Samed
- NABI Maya
- OZKOSAR Enes
- MONNERET Lucie
- LIDALT Omar


### To Do

- Improve AI efficiency.
- Add network play functionality with AI and GUI.
- Enhance the GUI with more visual elements.
