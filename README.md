# Assignment 2 — Data Structures Suite

Four independent data-structure modules compiled into one
interactive console program. Each module is accessible from
the main menu.

## Modules
| # | Name              | Data Structure          |
|---|-------------------|-------------------------|
| 1 | Window Manager    | Doubly Linked List      |
| 2 | Text Editor       | Dynamic Array (vector)  |
| 3 | White Area Finder | 2-D Grid + Recursive DFS|
| 4 | Binary Search Tree| BST + Queue (level-order)|

## File Structure
assignment2.cpp   — single self-contained source file

## Requirements
- C++11 or later
- Standard compiler: g++, clang++, MSVC

## Compilation
g++ -std=c++11 -Wall -o suite assignment2.cpp

## Running
./suite
# Select 1–4 from the main menu

---

## Q1 — Window Manager
Simulates a desktop window stack using a doubly linked list.
- Activating a window detaches it and moves it to the head (front = active)
- Supports: create, activate, resize, delete, display all
- Duplicate titles and invalid dimensions are rejected
- Pre-loaded with Browser, File Manager, Terminal on startup

## Q2 — Text Editor
A line-based text editor with a simulated cursor.
- File operations: new, open, save, save-as, close, recent files (last 8)
- Editing: type text, edit/delete/insert line, backspace, delete char
- Cursor: up/down/left/right/home/end/page-up/page-down
- Find & replace (all occurrences), select all, word wrap
- Formatting state: font name, text color, font size (display only)
- Unsaved-change guard on all destructive operations

## Q3 — White Area Finder
Counts disconnected white regions in an N×N binary grid (N up to 20).
- Input: 0 = white cell, 1 = black cell
- Algorithm: recursive 4-directional DFS (flood fill)
- Pads grid with a 1-cell black border to simplify boundary checks
- Reports number of regions and size of each
- Built-in tests: chessboard 4×4, all-black 3×3, all-white 3×3, 1×1

## Q4 — Binary Search Tree
Full BST implementation with an interactive menu and timing driver.
- Insert (duplicates silently ignored)
- Search (iterative O(log n) average)
- Delete: handles leaf, one-child, and two-child (inorder successor) cases
- Traversals: inorder (sorted), preorder, postorder, level-order
- Height query
- Timing driver: inserts 5 sets of 100 unique random integers,
  reports insertion time and inorder traversal time in microseconds
