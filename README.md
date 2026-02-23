# Connect-N

A generalized Connect Four game in C++ where two players compete to get N checkers in a row on a customizable grid.

## Overview

Players take turns dropping colored checkers (Red and Black) into columns of a scaffold. The first player to get N checkers in a row — horizontally, vertically, or diagonally — wins.

## Players

- **HumanPlayer** — prompts for input via the terminal
- **BadPlayer** — always picks the leftmost available column
- **SmartPlayer** — uses a minimax algorithm to play optimally

## Minimax Algorithm

The SmartPlayer uses a recursive minimax search that explores all possible future game states. It maximizes its own score while assuming the opponent minimizes it. Terminal states are rated as +10000 (win) or -10000 (loss), adjusted by depth so faster wins are preferred.

## How to Compile and Run
```bash
g++ main.cpp Game.cpp Player.cpp Scaffold.cpp -o connectn
./connectn
```

## Example
```cpp
SmartPlayer red("Red");
HumanPlayer black("Black");
Game g(4, 3, 3, &red, &black);
g.play();
```
