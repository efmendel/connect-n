// Scaffold.cpp

#include "provided.h"
#include <utility>
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class ScaffoldImpl
{
  public:
    ScaffoldImpl(int nColumns, int nLevels); 
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const; 
    void display() const; 
    bool makeMove(int column, int color); 
    int undoMove();
  private:
    int m_Columns;
    int m_Levels;
    stack<int> columnMoves;
    stack<int> levelMoves;
    vector<vector<string>> grid;
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels) : m_Columns(nColumns), m_Levels(nLevels)
{
    // first adjust number of rows
    grid.resize(nLevels);
    
    // iterate through the rows and adjust number of columns
    for (int i = 0; i < nLevels; i++)
        grid[i].resize(nColumns);
    
    // fill 2d vector all with empty slots (spaces)
    for (int i = 0; i < nLevels; i++) {
        for (int j = 0; j < nColumns; j++) {
            grid[i][j] = " ";
        }
    }
}

int ScaffoldImpl::cols() const
{
    return m_Columns;
}

int ScaffoldImpl::levels() const
{
    return m_Levels;
}

int ScaffoldImpl::numberEmpty() const
{
    // iterates through 2d vector and counts the number of spaces (slots with no chip)
    int total = 0;
    for (int i = 0; i < m_Levels; i++) {
        for (int j = 0; j < m_Columns; j++) {
            if (grid[i][j] == " ") {
                total++;
            }
        }
    }
    return total;
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    int row = m_Levels - level;  
    if (grid[row][column - 1] == "R") {
        return RED;
    } else if (grid[row][column - 1] == "B") {
        return BLACK;
    } else {
        return VACANT;
    }
}

void ScaffoldImpl::display() const
{
    // iterate through the number of rows to create each row
    for (int i = 0; i < m_Levels; i++) {
        // iterate through the number of columns to create a space for each column
        for (int j = 0; j < m_Columns; j++) {
            if (j == 0) {
                cout << "|";
            }
            cout << grid[i][j] << "|";
        }
            cout << endl;
    }
    
    // create the base, which consists always of one level and the number of columns in the game
    for (int j = 0; j < m_Columns; j++) {
        if (j == 0) {
            cout << "+";
        }
        cout << "-+";
    }
    
    cout << endl;
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    // out of scope of the game
    if (column < 1 || column > m_Columns) {
        return false;
    }
    // iterates from bottom to top so that piece is correctly slotted into the bottom most place
    // when piece is slotted, the move is added to the stack of moves to keep track of the game
    for (int i = m_Levels - 1; i > -1; i--) {
        if (grid[i][column - 1] == " ") {
            if (color == RED) {
                grid[i][column - 1] = "R";
                levelMoves.push(i);
                columnMoves.push(column - 1);
                return true;
            } else if (color == BLACK) {
                grid[i][column - 1] = "B";
                levelMoves.push(i);
                columnMoves.push(column - 1);
                return true;
            }
        }
    }
    return false;
}

int ScaffoldImpl::undoMove()
{
    // if the moves stack is empty, then the scaffold is empty and there is nothing to undo
    if (levelMoves.empty()) {
        return 0;
    }
    
    // returns the move, makes the space empty in the grid, and takes away those moves from the stack
    int lastColumn = columnMoves.top();
    grid[levelMoves.top()][lastColumn] = " ";
    levelMoves.pop();
    columnMoves.pop();
    return lastColumn;
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.  You should
//  not change any of the code from here to the end of this file.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}
 
Scaffold::~Scaffold()
{
    delete m_impl;
}
 
Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}
 
Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}
 
int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}
 
void Scaffold::display() const
{
    m_impl->display();
}
 
bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}
 
int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
