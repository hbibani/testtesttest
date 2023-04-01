#include "goalList.h"
#include "state.h"

#ifndef disjunctiveGoalList_H
#define disjunctiveGoalList_H

using namespace std;

class DisjunctiveGoalList : public GoalList {
  public:
    DisjunctiveGoalList() : GoalList() {};
    bool isSatisfied(State* gameState);
    void getActionHeuristic(State* gameState, Action* act);
};

// Runs through all the goals to see if any one of them are satisfied
bool DisjunctiveGoalList::isSatisfied(State* gameState) {
    for (list<Goal*>::iterator i = goalSet.begin(); i != goalSet.end(); i++) {
        if ((*i)->isSatisfied(gameState)) {
            cout << "The satisfied goal is: ";
            (*i)->showHumanReadable();
            return true;
        }
    }
    return false;
}


// Gets the heuristic for the action and applies it
// Takes the net minimum of all the heuristics and hence focuses on that goal
void DisjunctiveGoalList::getActionHeuristic(State* newGameState, Action* act) {
    double min = 1.0;
    for (list<Goal*>::iterator i = goalSet.begin(); i != goalSet.end(); i++) {
        double tmp = (*i)->getHeuristic(newGameState);
        if (tmp < min) {
            min = tmp;
        }
    }
    act->setHeuristic(min);
}

#endif
