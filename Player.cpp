
// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class HumanPlayerImpl
{
  public:
    HumanPlayerImpl(HumanPlayer* p);
    int chooseMove(const Scaffold& s, int N, int color);
  private:
    HumanPlayer* m_player;
};

class BadPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

HumanPlayerImpl::HumanPlayerImpl(HumanPlayer* p)
 : m_player(p)
{}

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    // requests input until a column that is wtihin the bounds of the game is given and returns that
    int move = -1;
    while (!(move > 0 && move < s.cols() + 1 && s.checkerAt(move, s.levels()) == VACANT)) {
        cout << m_player->name() << ", choose a valid move: ";
        cin >> move;
    }
    return move;
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    // retruns first non filled column left to right
    for (int move = 1; move <= s.cols(); move++) {
        if (s.checkerAt(move, 1) == VACANT) {
            return move;
        }

    }
    return -1;
}

bool completed(int& winner, const Scaffold& s, int winningNumber);
void determineBestComputerMove(Scaffold &s, int winningNumber, int& bestMove, int& moveScore, int depth, int color, int originalColor);
void determineBestHumanMove(Scaffold &s, int winningNumber, int& bestMove, int& moveScore, int depth, int color, int originalColor);

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    Scaffold argumentS = s;
    int returnMove;
    int returnMoveScore;
    determineBestComputerMove(argumentS, N, returnMove, returnMoveScore, 0, color, color);
    return returnMove;
}


int rating(Scaffold &s, int winningNumber, int depth, int originalComputerColor, bool isComputer, bool &gameDone) {
    int winner = -1;
    if (completed(winner, s, winningNumber)) {
        gameDone = true;
        if (winner == VACANT) {
            return 0;
        }
        
        int rating = 10000 - depth;
        
        // Always from original computer's perspective
        if (winner == originalComputerColor) {
            return rating;   // Good for computer
        } else {
            return -rating;  // Bad for computer
        }
    }
    gameDone = false;
    return 0;
}

void determineBestComputerMove(Scaffold &s, int winningNumber, int& bestMove, int& moveScore, int depth, int color, int originalColor) {
    vector<int> allMoves;           // Store ALL possible moves
    vector<int> allRatings;         // Store ALL corresponding ratings
    
    for (int i = 1; i <= s.cols(); i++) {
        bool gameDone = false;
        if (s.makeMove(i, color)) {
            int r = rating(s, winningNumber, depth, originalColor, true, gameDone);
            if (gameDone) {
                // Terminal position - use direct rating
                allMoves.push_back(i);
                allRatings.push_back(r);
            } else {
                // Non-terminal - get human's best response
                int bestHumanMove = -1;
                int humanMoveScore = 0;
                int otherColor = (color == RED) ? BLACK : RED;
                determineBestHumanMove(s, winningNumber, bestHumanMove, humanMoveScore, depth+1, otherColor, originalColor);
                allMoves.push_back(i);
                allRatings.push_back(humanMoveScore);  // Use human's returned score
            }
            s.undoMove();
        }
    }
    
    if (allMoves.empty()) {
        return;
    }
    
    // Computer maximizes - find the move with highest rating
    bestMove = allMoves[0];
    moveScore = allRatings[0];
    for (int i = 1; i < allMoves.size(); i++) {
        if (allRatings[i] > moveScore) {
            bestMove = allMoves[i];
            moveScore = allRatings[i];
        }
    }
}

void determineBestHumanMove(Scaffold &s, int winningNumber, int& bestMove, int& moveScore, int depth, int color, int originalColor) {
    vector<int> allMoves;
    vector<int> allRatings;
    
    for (int i = 1; i <= s.cols(); i++) {
        bool gameDone = false;
        if (s.makeMove(i, color)) {
            int r = rating(s, winningNumber, depth, originalColor, false, gameDone);
            if (gameDone) {
                // Terminal position
                allMoves.push_back(i);
                allRatings.push_back(r);
            } else {
                // Non-terminal - get computer's best response
                int bestComputerMove = -1;
                int computerMoveScore = 0;
                int otherColor = (color == RED) ? BLACK : RED;
                determineBestComputerMove(s, winningNumber, bestComputerMove, computerMoveScore, depth+1, otherColor, originalColor);
                allMoves.push_back(i);
                allRatings.push_back(computerMoveScore);  // Use computer's returned score
            }
            s.undoMove();
        }
    }
    
    if (allMoves.empty()) {
        return;
    }
    
    // Human minimizes - find the move with LOWEST rating (worst for computer)
    bestMove = allMoves[0];
    moveScore = allRatings[0];
    for (int i = 1; i < allMoves.size(); i++) {
        if (allRatings[i] < moveScore) {  // CHANGED: < instead of >
            bestMove = allMoves[i];
            moveScore = allRatings[i];
        }
    }
}

// copied completed code as a helper function from game file in order to check wins
// see explanation comments in game.cpp
bool completed(int& winner, const Scaffold& s, int winningNumber)
{
    
    // vertical checking (column by column)
    for (int i = 0; i < s.cols(); i++) {
        int rInARow = -1;
        int bInARow = -1;
        for (int j = 0; j < s.levels(); j++) {
            if (s.checkerAt(i + 1, j + 1) == RED) {
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
            if (s.checkerAt(i + 1, j + 1) == BLACK) {
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
            if (s.checkerAt(i + 1, j + 1) == VACANT) {
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
    for (int i = 0; i < s.levels(); i++) {
        int rInARow = -1;
        int bInARow = -1;
        for (int j = 0; j < s.cols(); j++) {
            if (s.checkerAt(j + 1, i + 1) == RED) {
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
            if (s.checkerAt(j + 1, i + 1) == BLACK) {
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
            if (s.checkerAt(j + 1, i + 1) == VACANT) {
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
    for (int i = 0; i < s.cols(); i++) {
        int rInARow = -1;
        int bInARow = -1;
        int currentRow = 0;
        int currentCol = i;
        while (currentCol < s.cols() && currentRow < s.levels()) {
            if (s.checkerAt(currentCol + 1, currentRow + 1) == RED) {
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
            if (s.checkerAt(currentCol + 1, currentRow + 1) == BLACK) {
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
            if (s.checkerAt(currentCol + 1, currentRow + 1) == VACANT) {
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
    for (int i = 1; i < s.levels(); i++) {
        int rInARow = -1;
        int bInARow = -1;
        int currentRow = i;
        int currentCol = 0;
        while (currentCol < s.cols() && currentRow < s.levels()) {
            if (s.checkerAt(currentCol + 1, currentRow + 1) == RED) {
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
            if (s.checkerAt(currentCol + 1, currentRow + 1) == BLACK) {
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
            if (s.checkerAt(currentCol + 1, currentRow + 1) == VACANT) {
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
    for (int i = 0; i < s.cols(); i++) {
        int rInARow = -1;
        int bInARow = -1;
        int currentRow = 0;
        int currentCol = i;
        while (currentCol >= 0 && currentRow < s.levels()) {
            if (s.checkerAt(currentCol + 1, currentRow + 1) == RED) {
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
            if (s.checkerAt(currentCol + 1, currentRow + 1) == BLACK) {
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
            if (s.checkerAt(currentCol + 1, currentRow + 1) == VACANT) {
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
    for (int i = 1; i < s.levels(); i++) {
        int rInARow = -1;
        int bInARow = -1;
        int currentRow = i;
        int currentCol = s.cols() - 1;
        while (currentCol >= 0 && currentRow < s.levels()) {
            if (s.checkerAt(currentCol + 1, currentRow + 1) == RED) {
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
            if (s.checkerAt(currentCol + 1, currentRow + 1) == BLACK) {
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
            if (s.checkerAt(currentCol + 1, currentRow + 1) == VACANT) {
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
    
    if (s.numberEmpty() == 0) {
        winner = VACANT;
        return true;
    }
    
    // after all checking there's no true, then return false
    winner = VACANT;
    return false;
}

//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.  You should
//  not change any of the code from here to the end of this file.

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl(this);
}
 
HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}
 
int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}
 
BadPlayer::~BadPlayer()
{
    delete m_impl;
}
 
int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}
 
SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}
 
int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

