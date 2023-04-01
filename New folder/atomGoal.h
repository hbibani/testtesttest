#include <iostream>

#include "goal.h"

using namespace std;

#ifndef atomGoal_H
#define atomGoal_H

class AtomGoal : public Goal {
  public:
    AtomGoal(State* g) : Goal(
      getRand(1, g->getNums()),
      getRand(0, g->getSize()-1),
      getRand(0, g->getSize()-1)
    ) {};
    AtomGoal(int a, int b, int c) : Goal(a, b, c) {};

    void showHumanReadable();
    bool isValid(State* gameState);
    bool isSatisfied(State* gameState);
    double getHeuristic(State* gameState);
};

// Shows a human readable atom goal
void AtomGoal::showHumanReadable() {
    cout << "Tile " << goalTuple[0] << " must be at: row " << goalTuple[1] << ", col " << goalTuple[2] << endl;
}


// Checks whether the atom goal is valid
bool AtomGoal::isValid(State* gameState) {
    return (
        goalTuple[0] > 0 &&     // Make sure the tile is non-zero
        goalTuple[0] <= gameState->getNums() &&   // Make sure the tile actually exists
        goalTuple[1] >= 0 &&    // Check the lower bound of the coordinate
        goalTuple[1] < gameState->getSize() &&   // Check upper bound
        goalTuple[2] >= 0 &&    // Check the lower bound of the coordinate
        goalTuple[2] < gameState->getSize()     // Check upper bound
        );
}


// Uses the game state to check whether the goal is satisfied
bool AtomGoal::isSatisfied(State* gameState) {
    int** tempBoard = gameState->getBoard();
    return goalTuple[0] == tempBoard[goalTuple[1]][goalTuple[2]];
}


// Gets the heuristic for an action based on how
double AtomGoal::getHeuristic(State* gameState) {
    int x, y;
    gameState->find(goalTuple[0], x, y);
    // Take the linear distance from the number to the goal and then
    // normalise it between 0 and 1 by dividing it by the maximum distance.
    //return sqrt(pow((double)x-goalTuple[1], 2) + pow((double)y-goalTuple[2], 2))/gameState->maxHeuristic;
    return linDist(x, goalTuple[1], y, goalTuple[2]) / gameState->maxHeuristic;
}


#endif
