# Turkish Checkers (Dama) Game and AI Engine (Sombol1.0)

This repository contains a Python implementation of the game along with a highly optimized AI engine written in C++.

## About the Game

Turkish Checkers, commonly known as Dama, is a popular board game played on an 8x8 board with pieces placed on dark squares. The game follows specific rules for movement and capturing, providing an engaging strategic challenge for players.

## Features

- **Python Implementation**: The game logic and interface are implemented in Python, providing an accessible platform for playing the game.
- **C++ AI Engine**: The repository includes a powerful AI engine written in C++, capable of providing challenging gameplay experiences. The AI engine is highly optimized for efficiency and effectiveness.
- **Interactive Gameplay**: Players can enjoy interactive gameplay by running the `dama.py` Python script. Simply execute `python dama.py` to start a new game and play against the AI engine.
- **New Game Options**: The game interface allows players to start new games and select various options, providing flexibility and customization for gameplay.
- **Rules Enforcement**: The game strictly follows the rules of Turkish Checkers, ensuring an authentic and fair gaming experience.

## Engine Features
 - bitboard board representation
 - Transposition Table
 - encoding moves and pieces as 8-bit numbers
 - copy/make approach for making moves
 - minimax search with alpha beta pruning
 - PV/killer/history move ordering
 - iterative deepening
 - PVS (Principle Variation Search)

## Getting Started

To play the Turkish Checkers game, follow these steps:

1. Clone the repository to your local machine.
2. Ensure you have Python and a C++ compiler (with the needed libraries) installed on your system.
3. If you only plan on playing the engine and not checking the code you can install only the game from [Dama](./github.com/HadiKhansaa/dama).
4. Install pygame by running this command: `pip install pygame`.
5. Navigate to the repository directory.
6. Run the `dama.py` script using Python: `python dama.py`.
7. Use the GUI to choose your best settings and you're ready to go!
