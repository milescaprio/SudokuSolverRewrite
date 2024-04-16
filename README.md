- A program to solve sudoku puzzles, using two human-like algorithms. Rewrite in C++ to clean up my original 2018 Python attempt. Uses functions from my own C++ library.

- Cannot solve all sudoku puzzles that can be deterministically solved, however never renders an incorrect answer.

Algorithm 1: Checks each square and counts candidate numbers for that square: if only one candidate, place it.
Algorithm 2: Checks each set (row, column, and section) and counts candidate locations for each number: if only one candidate location, place number there.

Last modified May 2022, later uploaded to GitHub
