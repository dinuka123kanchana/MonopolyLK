
# Monopoly-LK

A Monopoly game simulation developed in C as an individual assignment for SCS 1301 – Data Structures and Program Design at the University of Colombo School of Computing (UCSC).

The program simulates a Monopoly-style game with four AI-controlled players, featuring property ownership, financial transactions, board movement, and a maximum of 500 rounds.

## Features

- Four AI-controlled players
- 40-square game board
- Property management (22 properties, 4 railways, and 2 utilities)
- Player movement and board navigation
- Financial transactions and rent payments
- Event-based game mechanics
- Round-by-round game output
- Final game results

## Technologies

- C
- GCC
- Visual Studio Code

## Project Structure

```text
Monopoly-LK/
|
|-- types.h
|-- board.c
|-- players.c
|-- finance.c
|-- events.c
|-- game.c
|-- main.c
|-- README.md
```

## Compilation and Execution

Compile the program using GCC:

```bash
gcc main.c board.c players.c finance.c events.c game.c -o monopoly
```

Run on Windows:

```bash
.\monopoly.exe
```

Run on Linux/macOS:

```bash
./monopoly
```

## Academic Information

Course: SCS 1301 – Data Structures and Program Design

Institution: University of Colombo School of Computing

Project Type: Individual Assignment

## Author

Angadhe Dhinuka Kanchana Perera

GitHub: https://github.com/dinuka123kanch

LinkedIn: https://www.linkedin.com/in/dinuka-kanchana-968992355/
