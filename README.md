# Conway's Game of Life in C

This project implements Conway's Game of Life in C, with support for multiple tasks including simple generation updates, tracking cell changes, and a binary tree of game generations.

## Features

- **Task 1**: Simulates `K` generations using the standard Game of Life rules.
- **Task 2**: Tracks and displays only the cells that change between generations. Supports bonus reversal to generation 0.
- **Task 3**: Constructs a binary tree of generations where:
  - Left child uses Rule B (any cell with exactly 2 live neighbors becomes alive).
  - Right child uses the standard Game of Life rule.
  - Generations are built up to depth `K`, and the entire tree is printed in pre-order.

`For more details access:` https://site-pa.netlify.app/proiecte/game_of_life/

## Build Instructions

Use the provided `Makefile` to compile:

```bash
make
```
## File Structure
test.c — Main source code.

Makefile — For compiling the project.

README.md — Project documentation.

## Important Data Structures
`Node`: Doubly linked list element used for storing cell coordinates and states.

`tree`: Structure for representing a node in the binary generation tree (Task 3).

## Core Functions
`nrvecinivii`: Counts the number of live neighbors for a cell.

`calc_changes`: Computes the changes from one generation to the next based on the rule.

`changes`: Applies a list of cell changes to a matrix.

`construieste_arbore`: Recursively builds the binary generation tree.

`afisare_final`: Prints the tree using pre-order traversal.

`t2bonus`: Reverses changes to recover the initial generation (bonus for Task 2).
