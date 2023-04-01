#include <iostream>
#include <string>
#include <math.h>

#include "constants.h"
#include "randomness.h"
#include "state.h"

#ifndef goal_H
#define goal_H

class Goal {
  protected:
    int goalTuple[3];

  public:
    Goal() {};
    Goal(int a, int b, int c);
    string toString();
    void show();
    double linDist(int x0, int x1, int y0, int y1) {
      return sqrt(pow((double)x0-x1, 2) + pow((double)y0-y1, 2));
    };
    virtual void showHumanReadable() = 0;
    virtual bool isValid(State* gameState) = 0;
    virtual bool isSatisfied(State* gameState) = 0;
    virtual double getHeuristic(State* gameState) = 0;
};

// Constructor sets up the tuple
// The tuple holds the coordinates and directions of the goal
// They hold different things depending on an atom or neighbour goal
Goal::Goal(int a, int b, int c) {
    goalTuple[0] = a;
    goalTuple[1] = b;
    goalTuple[2] = c;
}


// Converts the goal to a string by simply printing the tuple array
string Goal::toString() {
    // Convert the direction provided in a neighbour goal if any
    string directionOrCoord;
    if (goalTuple[1] < 0) {
        directionOrCoord = DIRECTION_CHARS[4 + goalTuple[1]];
    }
    else {
        directionOrCoord = to_string(goalTuple[1]);
    }

    return "(" + to_string(goalTuple[0]) + ", " + directionOrCoord + ", " + to_string(goalTuple[2]) + ")";
}


// Show the non-human-readable presentation of the goal
void Goal::show() {
    cout << toString() << endl;
}


#endif
