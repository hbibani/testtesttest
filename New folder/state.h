#include <vector>
#include <math.h>
#include <assert.h>

#include "constants.h"
#include "action.h"
#include "randomness.h"
#include <algorithm>    // std::random_shuffle


#ifndef state_H
#define state_H

using namespace std;


class State {
  int** board;
  int size;
  int nums;

  int top(int col);
  bool isEmpty(int col);
  int popFromCol(int col);

  public:
    double maxHeuristic;

    State() {}; // Empty constructor (do not construct like this unless it is a placeholder)
    State(State* s); // Copy constructor
    State(int s, int n);
    State(int s, int n, int* boardDef);
    void initBoard();
    void clearBoard();
    void randomiseBoard();
    int getSize() {return size;};
    int getNums() {return nums;};
    int** getBoard() {return board;}
    void find(int num, int& x, int& y);
    int topTile(int col);
    void showBoard();
    void pushToCol(int val, int col); // Must remain public for manual board description
    bool isValidAction(Action& a);
    void performAction(Action& a);
    void reverseAction(Action& a);
    void getPossibleMoves(vector<Action>& actionList);
    string getHash();
    ~State();
};


// Copy constructor
State::State(State* s) {
    size = s->getSize();
    nums = s->getNums();
    // Initialise the memory for the board
    initBoard();
    // Copy the board
    int** otherBoard = s->getBoard();
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            board[x][y] = otherBoard[x][y];
        }
    }
    // Get the heuristic
    maxHeuristic = s->maxHeuristic;
}


// Starts a state with a completely random board
// s is the size of the board (eg s=3, it will create a 3x3 board)
// n is the amount of numbers to place on the board
State::State(int s, int n) {
    size = s;
    nums = n;
    maxHeuristic = sqrt(2.0 * pow((double)size, 2.0));
    assert(size >= 2 && size <= nums && nums <= size * size - size);
    initBoard();
    randomiseBoard();
}


State::State(int s, int n, int* boardDef) {
    size = s;
    nums = n;
    maxHeuristic = sqrt(2.0 * pow((double)size, 2.0));
    assert(size >= 2 && size <= nums && nums <= size * size - size);
    initBoard();

    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            board[x][y] = boardDef[(x * size) + y];
        }
    }
}


// Zeroes-out the board and initialises memory, ready for game setup
void State::initBoard() {
    board = new int* [size];
    for (int x = 0; x < size; x++) {
        board[x] = new int[size];
        for (int y = 0; y < size; y++) {
            board[x][y] = 0;
        }
    }
}


// Zeroes-out the board
void State::clearBoard() {
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            board[x][y] = 0;
        }
    }
}


void State::randomiseBoard() {
    // Get a list of all the nums
    vector<int> shuffledNums;
    for (int i = 1; i <= nums; i++) {
        shuffledNums.push_back(i);
    }
    // Shuffle them
    random_shuffle(shuffledNums.begin(), shuffledNums.end());
    // Iterate through the shuffled vector
    int iterations, currentCol = 0;
    for (vector<int>::iterator i = shuffledNums.begin(); i != shuffledNums.end(); i++) {
        // Get a random number of iterations (the larger the number, the more random it is)
        iterations = getRand(10, 10 * size);
        // Keep iterating for `iterations` and until you come across a free space
        while (iterations > 0 || top(currentCol) != 0) {
            // Decrement only on free space
            if (top(currentCol) == 0) {
                iterations--;
            }
            currentCol++;
            currentCol = currentCol % size;
        }
        // Put the number in that column
        pushToCol(*i, currentCol);
    }
}


// Finds the value `num` and puts the coordinates into x and y
// Else x=0, y=0
void State::find(int num, int& x, int& y) {
    x = 0;
    y = 0;

    for (int cx = 0; cx < size; cx++) {
        for (int cy = 0; cy < size; cy++) {
            if (board[cx][cy] == num) {
                x = cx;
                y = cy;
                return;
            }
        }
    }
}


// Gets the item at the top of a column (the very top, not the top of the stack)
// Zero means that it has space
int State::top(int col) {
    assert(col < size && col >= 0);
    return board[size - 1][col];
}


// Gets the top-most tile in the column else returns zero
// Zero means it is empty
// Algorithm keeps iterating until it reaches the bottom of a column or a tile
int State::topTile(int col) {
    if (isEmpty(col)) {
        return 0;
    }
    else {
        // Note: we could just pop and push back but that would be inefficient
        int i = size - 1;
        int current = board[i][col];
        while (current == 0 && i > 0) {
            i--;
            current = board[i][col];
        }
        return current;
    }
}


// Checks if there is a tile in a column
bool State::isEmpty(int col) {
    assert(col < size && col >= 0);
    return board[0][col] == 0;
}


// Pushes a value into a column only if it has space
// Algorithm keeps iterating until it reaches the bottom of a column or a zero
// before a tile
void State::pushToCol(int val, int col) {
    assert(top(col) == 0 && val > 0 && val <= nums);
    int i = 0;
    int current = board[0][col];
    while (current != 0 && i < size - 1) {
        i++;
        current = board[i][col];
    }

    if (current == 0) {
        board[i][col] = val;
    }
}


// Pops and returns a value from a column only if a tile exists
// Algorithm keeps iterating until it reaches the bottom of a column or a zero
// before a tile
int State::popFromCol(int col) {
    assert(!isEmpty(col));
    int i = size - 1;
    int current = board[i][col];
    while (current == 0 && i > 0) {
        i--;
        current = board[i][col];
    }

    if (current != 0) {
        int temp = board[i][col];
        board[i][col] = 0;
        return temp;
    }
    else {
        return 0;
    }
}


// Checks if an action is valid and can be performed
bool State::isValidAction(Action& a) {
    return (a.getFromCol() < size && a.getFromCol() >= 0) && // Validate bounds
        (a.getToCol() < size && a.getToCol() >= 0) && // Validate bounds
        (a.getFromCol() != a.getToCol()) && // Ensure the action is not wasteful
        (!isEmpty(a.getFromCol())) && // Check if there is a value to move
        (top(a.getToCol()) == 0); // Check if there is a free place to put it
}


// Pop the value from one column and push it to another
void State::performAction(Action& a) {
    pushToCol(popFromCol(a.getFromCol()), a.getToCol());
}


// Reverse of performAction
void State::reverseAction(Action& a) {
    pushToCol(popFromCol(a.getToCol()), a.getFromCol());
}


// Get all the possible moves that can be performed
void State::getPossibleMoves(vector<Action>& actionList) {
    // Iterate through all the columns
    for (int col = 0; col < size; col++) {
        // Actions can only occur from a column with tiles, skip empty columns
        if (!isEmpty(col)) {
            // See what other columns can be pushed to by iterating through them again
            for (int otherCol = 0; otherCol < size; otherCol++) {
                // If it is not the same column and it has room
                if (otherCol != col && top(otherCol) == 0) {
                    // An action can push a tile to this column
                    // So add it to the final list
                    actionList.push_back(Action(col, otherCol));
                }
            }
        }
    }
}


// Convert all numbers on the board to a string
// This will make a unique identifier for a particular state without
// having to program an equality overloader
string State::getHash() {
    string hash = "";
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            hash += (char)board[x][y] + 48;
        }
    }
    return hash;
}


void State::showBoard() {
    int mag = floor(log10(nums));
    // Push to a new line
    cout << endl;
    // Re-usable separators
    string preRowSep = "";
    for (int i = 0; i < size; i++) {
        preRowSep += "|";
        preRowSep += string(3 + mag, ' ');
    }
    preRowSep += "|\n";

    string rowSep = "+";
    for (int i = 0; i < size; i++) {
        rowSep += string(3 + mag, '-');
        rowSep += "+";
    }
    rowSep += "\n";


    // Start from top to bottom (x is rows)
    for (int x = size - 1; x >= 0; x--) {
        cout << preRowSep;
        for (int y = 0; y < size; y++) {
            if (board[x][y] != 0) {
                cout << "| " << board[x][y] << string(1 + mag - (int)floor(log10(board[x][y])), ' ');
            }
            else {
                cout << "|" << string(3 + mag, ' ');
            }
        }
        // Fix off-by-one bug
        cout << "|" << endl;
        cout << preRowSep;
        cout << rowSep;
    }
}


State::~State() {
    for (int x = 0; x < size; x++) {
        delete[] board[x];
    }

    delete[] board;
}


#endif
