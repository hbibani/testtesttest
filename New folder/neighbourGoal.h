#include <iostream>

#include "goal.h"

using namespace std;

#ifndef neighbourGoal_H
#define neighbourGoal_H

class NeighbourGoal : public Goal {
  public:
    NeighbourGoal(State* g);
    NeighbourGoal(int a, int b, int c) : Goal(a, b, c) {};

    void showHumanReadable();
    bool isValid(State* gameState);
    bool isSatisfied(State* gameState);
    double getHeuristic(State* gameState);
};


// The random constructor that generates a random neighbour goal
// It requires a state be passed in to ensure that the parameters are valid
NeighbourGoal::NeighbourGoal(State* g) {
    goalTuple[0] = getRand(1, g->getNums());
    goalTuple[1] = -getRand(-ABOVE, -RIGHT);
    // While the second number is not the same as the first
    // Prevents an impossible goal
    do {
        goalTuple[2] = getRand(1, g->getNums());
    } while (goalTuple[0] == goalTuple[2]);
}


// Presents a human readable string describing the goal
void NeighbourGoal::showHumanReadable() {
    cout << "Tile " << goalTuple[0] << " must be " << DIRECTION_STRS[goalTuple[1] + 4] << " tile " << goalTuple[2] << endl;
}


// Checks whether the goal is valid
bool NeighbourGoal::isValid(State* gameState) {
    return (
        goalTuple[0] > 0 &&     // Make sure the tile is non-zero
        goalTuple[0] <= gameState->getNums() &&   // Make sure the tile actually exists
        goalTuple[1] < 0 &&     // Direction is negative
        goalTuple[1] > -5 &&    // Direction is no less than -4
        goalTuple[2] > 0 &&     // Make sure the tile is non-zero
        goalTuple[2] <= gameState->getNums() &&   // Make sure the tile actually exists
        goalTuple[0] != goalTuple[2]    // Make sure we don't have the goal referenceing the same number
        );
}


// Checks whether the goal is satisfied given a particular state
bool NeighbourGoal::isSatisfied(State* gameState) {
    // Initialise the board parameters
    int x, y, size = gameState->getSize();
    int** tempBoard = gameState->getBoard();
    // Find the coordinates of the target number
    gameState->find(goalTuple[2], x, y);

    // Check which direction the neighbour is and check whether the
    // target number is equal to that cell
    if (goalTuple[1] == ABOVE && x + 1 < size) {
        return tempBoard[x + 1][y] == goalTuple[0];
    }
    else if (goalTuple[1] == BELOW && x - 1 >= 0) {
        return tempBoard[x - 1][y] == goalTuple[0];
    }
    else if (goalTuple[1] == LEFT && y - 1 >= 0) {
        return tempBoard[x][y - 1] == goalTuple[0];
    }
    else if (goalTuple[1] == RIGHT && y + 1 < size) {
        return tempBoard[x][y + 1] == goalTuple[0];
    }

    // Return false if the target is outside the board
    return false;
}


// Get the heuristic for a neighbour goal based on a state
// Takes the linear distance between the base number and the neighbour position
// If a target neighbour is off the board or occupied, the heuristic is 1
double NeighbourGoal::getHeuristic(State* gameState) {
    // Get the board parameters
    int x, y, goalNumX, goalNumY, size = gameState->getSize();
    int** tempBoard = gameState->getBoard();
    double finalH = 1.0; // Set up the worst case heuristic
    // Get the coordinates of the base goal and the neighbour
    gameState->find(goalTuple[2], x, y);
    gameState->find(goalTuple[0], goalNumX, goalNumY);

    // For the particular direction, get the linear distance from the base goal
    // to the neighbour if the space is unoccupied
    if (goalTuple[1] == ABOVE && x + 1 < size) {
        if (tempBoard[x + 1][y] == 0) {
            finalH = linDist(x + 1, goalNumX, y, goalNumY);
        }
    }
    else if (goalTuple[1] == BELOW && x - 1 >= 0) {
        // It is impossible to have a tile below = 0
        // Simply get the linear distance
        finalH = linDist(x - 1, goalNumX, y, goalNumY);
    }
    else if (goalTuple[1] == LEFT && y - 1 >= 0) {
        if (tempBoard[x][y - 1] == 0) {
            finalH = linDist(x, goalNumX, y - 1, goalNumY);
        }
    }
    else if (goalTuple[1] == RIGHT && y + 1 < size) {
        if (tempBoard[x][y + 1] == 0) {
            finalH = linDist(x, goalNumX, y + 1, goalNumY);
        }
    }
    return finalH;
}


#endif
