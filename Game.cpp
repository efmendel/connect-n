// Game.cpp

#include "provided.h"
#include <iostream>

class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black); 
    bool completed(int& winner) const; 
    bool takeTurn(); 
    void play(); 
    int checkerAt(int c, int r) const;
  private:
    Scaffold s;
    int winningNumber;
    Player* red;
    Player* black;
    Player* lastTurn;
};

// constructor properly creates the scaffold, and properly sets all other variables, with lastTurn being a nullptr since no one has gone yet at the start
GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black) : s(nColumns, nLevels), winningNumber(N), red(red), black(black), lastTurn(nullptr)
{
}

bool GameImpl::completed(int& winner) const
{
    
    // vertical checking (column by column)
    // iterates through column by column
    for (int i = 0; i < s.cols(); i++) {
        // reset the counter at every new column
        int rInARow = -1;
        int bInARow = -1;
        // iterates through the rows (vertical fashion)
        for (int j = 0; j < s.levels(); j++) {
            // either continues the streak, starts a streak, or breaks a streak depending on what is in the given slot
            if (s.checkerAt(i + 1, s.levels() - j) == RED) {
                if (rInARow == -1) {
                    rInARow = 1;
                } else {
                    rInARow ++;
                }
                
                if (bInARow != -1) {
                    bInARow = -1;
                }
                
                if (winningNumber == rInARow) {
                    winner = RED;
                    return true;
                }
            }
            if (s.checkerAt(i + 1, s.levels() - j) == BLACK) {
                if (bInARow == -1) {
                    bInARow = 1;
                } else {
                    bInARow ++;
                }
                
                if (rInARow != -1) {
                    rInARow = -1;
                }
                
                if (winningNumber == bInARow) {
                    winner = BLACK;
                    return true;
                }
            }
            if (s.checkerAt(i + 1, s.levels() - j) == VACANT) {
                if (rInARow != -1) {
                    rInARow = -1;
                }
                if (bInARow != -1) {
                    bInARow = -1;
                }
            }
        }
    }
    
    // horizontal checking (row by row)
    // iterates through row by row
    for (int i = 0; i < s.levels(); i++) {
        // resets the counter at every new row
        int rInARow = -1;
        int bInARow = -1;
        // iterates through columns (horizontal fashion)
        for (int j = 0; j < s.cols(); j++) {
            // either continues the streak, starts a streak, or breaks a streak depending on what is in the given slot
            if (s.checkerAt(j + 1, s.levels() - i) == RED) {
                if (rInARow == -1) {
                    rInARow = 1;
                } else {
                    rInARow ++;
                }
                
                if (bInARow != -1) {
                    bInARow = -1;
                }
                
                if (winningNumber == rInARow) {
                    winner = RED;
                    return true;
                }
            }
            if (s.checkerAt(j + 1, s.levels() - i) == BLACK) {
                if (bInARow == -1) {
                    bInARow = 1;
                } else {
                    bInARow ++;
                }
                
                if (rInARow != -1) {
                    rInARow = -1;
                }
                
                if (winningNumber == bInARow) {
                    winner = BLACK;
                    return true;
                }
            }
            if (s.checkerAt(j + 1, s.levels() - i) == VACANT) {
                if (rInARow != -1) {
                    rInARow = -1;
                }
                if (bInARow != -1) {
                    bInARow = -1;
                }
            }
        }
    }
    
    // diagonal checking (negative slope)
    //   upper half:
    // top half negative slope diagonals all start from the first level and trickle down by traveling one right and one down
    // iterate by column
    for (int i = 0; i < s.cols(); i++) {
        // reset count at every diagonal
        int rInARow = -1;
        int bInARow = -1;
        int currentRow = 0;
        int currentCol = i;
        // keep going down the diagonal until out of bounds
        while (currentCol < s.cols() && currentRow < s.levels()) {
            // either continues the streak, starts a streak, or breaks a streak depending on what is in the given slot
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == RED) {
                if (rInARow == -1) {
                    rInARow = 1;
                } else {
                    rInARow ++;
                }
                
                if (bInARow != -1) {
                    bInARow = -1;
                }
                
                if (winningNumber == rInARow) {
                    winner = RED;
                    return true;
                }
            }
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == BLACK) {
                if (bInARow == -1) {
                    bInARow = 1;
                } else {
                    bInARow ++;
                }
                
                if (rInARow != -1) {
                    rInARow = -1;
                }
                
                if (winningNumber == bInARow) {
                    winner = BLACK;
                    return true;
                }
            }
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == VACANT) {
                if (rInARow != -1) {
                    rInARow = -1;
                }
                if (bInARow != -1) {
                    bInARow = -1;
                }
            }
            currentRow++;
            currentCol++;
        }
    }
    //   lower half:
    // lower half of negative diagonals all start at the first column, but on different levels
    // iterate through the levels to capture all lower diagonals
    for (int i = 1; i < s.levels(); i++) {
        // reset counter at every diagonal
        int rInARow = -1;
        int bInARow = -1;
        int currentRow = i;
        int currentCol = 0;
        // go until iterator goes out of the grid's bounds
        while (currentCol < s.cols() && currentRow < s.levels()) {
            // either continues the streak, starts a streak, or breaks a streak depending on what is in the given slot
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == RED) {
                if (rInARow == -1) {
                    rInARow = 1;
                } else {
                    rInARow ++;
                }
                
                if (bInARow != -1) {
                    bInARow = -1;
                }
                
                if (winningNumber == rInARow) {
                    winner = RED;
                    return true;
                }
            }
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == BLACK) {
                if (bInARow == -1) {
                    bInARow = 1;
                } else {
                    bInARow ++;
                }
                
                if (rInARow != -1) {
                    rInARow = -1;
                }
                
                if (winningNumber == bInARow) {
                    winner = BLACK;
                    return true;
                }
            }
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == VACANT) {
                if (rInARow != -1) {
                    rInARow = -1;
                }
                if (bInARow != -1) {
                    bInARow = -1;
                }
            }
            currentRow++;
            currentCol++;
        }
    }
    
    // diagonal checking (positive slope)
    //   upper half:
    // code is exactly same as above upper half diagonal check except, iterating trickles down left as it goes down not right
    for (int i = 0; i < s.cols(); i++) {
        int rInARow = -1;
        int bInARow = -1;
        int currentRow = 0;
        int currentCol = i;
        while (currentCol >= 0 && currentRow < s.levels()) {
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == RED) {
                if (rInARow == -1) {
                    rInARow = 1;
                } else {
                    rInARow ++;
                }
                
                if (bInARow != -1) {
                    bInARow = -1;
                }
                
                if (winningNumber == rInARow) {
                    winner = RED;
                    return true;
                }
            }
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == BLACK) {
                if (bInARow == -1) {
                    bInARow = 1;
                } else {
                    bInARow ++;
                }
                
                if (rInARow != -1) {
                    rInARow = -1;
                }
                
                if (winningNumber == bInARow) {
                    winner = BLACK;
                    return true;
                }
            }
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == VACANT) {
                if (rInARow != -1) {
                    rInARow = -1;
                }
                if (bInARow != -1) {
                    bInARow = -1;
                }
            }
            currentRow++;
            currentCol--;
        }
    }
    
    //   lower half:
    // code is exactly same as above lower half diagonal check except, iterating trickles down left as it goes down not right
    for (int i = 1; i < s.levels(); i++) {
        int rInARow = -1;
        int bInARow = -1;
        int currentRow = i;
        int currentCol = s.cols() - 1;
        while (currentCol >= 0 && currentRow < s.levels()) {
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == RED) {
                if (rInARow == -1) {
                    rInARow = 1;
                } else {
                    rInARow ++;
                }
                
                if (bInARow != -1) {
                    bInARow = -1;
                }
                
                if (winningNumber == rInARow) {
                    winner = RED;
                    return true;
                }
            }
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == BLACK) {
                if (bInARow == -1) {
                    bInARow = 1;
                } else {
                    bInARow ++;
                }
                
                if (rInARow != -1) {
                    rInARow = -1;
                }
                
                if (winningNumber == bInARow) {
                    winner = BLACK;
                    return true;
                }
            }
            if (s.checkerAt(currentCol + 1, s.levels() - currentRow) == VACANT) {
                if (rInARow != -1) {
                    rInARow = -1;
                }
                if (bInARow != -1) {
                    bInARow = -1;
                }
            }
            currentRow++;
            currentCol--;
        }
    }
    
    // if the scaffold doesn't have any more open slots and no wins are detected, then it's a tie, but the game is completed
    if (s.numberEmpty() == 0) {
        winner = VACANT;
        return true;
    }
    
    // after all checking and the board isn't all the way filled up, the game has not ended yet
    winner = VACANT;
    return false;
}

bool GameImpl::takeTurn()
{
    int temp = 0;
    // if game is done no turn is needed
    if(completed(temp)) {
        return false;
    }
    // red goes first or if black just went
    if (lastTurn == nullptr || lastTurn == black) {
        s.makeMove(red->chooseMove(s, winningNumber, RED), RED);
        lastTurn = red;
        return true;
    } else {
        s.makeMove(black->chooseMove(s, winningNumber, BLACK), BLACK);
        lastTurn = black;
        return true;
    }
}

void GameImpl::play()
{
    // set winner variable
    int winner = -1;
    s.display();
    
    // while has not finished, take take turns and display the scaffold after each turn
    while (!completed(winner)) {
        takeTurn();
        s.display();
    }
    
    // outputs the outcome of the game
    if (winner == RED)
        std::cout << "Red wins!" << std::endl;
    else if (winner == BLACK)
        std::cout << "Black wins!" << std::endl;
    else
        std::cout << "It's a tie!" << std::endl;
}

int GameImpl::checkerAt(int c, int r) const
{
    return s.checkerAt(c, r);
}


//******************** Game functions **************************************

//  These functions simply delegate to GameImpl's functions.  You should not
//  change any of the code from here to the end of this file.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
