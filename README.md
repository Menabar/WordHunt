# What is Word Hunt?

Word Hunt is an iMessage game similar to Boggle. The goal of the game is to find words in a grid of letters.

# What does this program do?

This program solves a Word Hunt grid. A copy of the board is printed to the terminal, followed by all found words.

# How to run the program.

The grid to be solved is input using the file `input.txt`. Each letter goes on a separate line and corresponds to the grid starting from left to right, top to bottom.

This repo contains an executable file called `run`. Use the command `./run < input.txt` to solve the board defined in `input.txt`.

To print found words to an output file instead, use the command `./run < input.txt > output.txt`. A copy of the board will still be printed to the terminal.

# Making your own changes.

## Recompiling

To compile, run the command `g++ -std=c++11 wordhunt.cc -o run`.

## Changing Valid Words

To change valid words:
- Add, remove, or edit the valid words in `english_words.txt`.
- Run the command `./update`.

## Changing Board Size

The board width and heigth are defined by constants `_width` and `_height` in `wordhunt.h`. The board can be any rectangle, as long as the number of letters in `input.txt` is equal to the number of cells in the board.