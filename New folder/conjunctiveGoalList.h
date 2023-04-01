#include "goalList.h"
#include "state.h"

#ifndef conjunctiveGoalList_H
#define conjunctiveGoalList_H

using namespace std;

class ConjunctiveGoalList : public GoalList {
  public:
    ConjunctiveGoalList() : GoalList() {};
    bool isSatisfied(State* gameState);
    void getActionHeuristic(State* gameState, Action* act);
};
// Runs through the goal list to see if all of the goals are satisfied
bool ConjunctiveGoalList::isSatisfied(State* gameState) {
    for (list<Goal*>::iterator i = goalSet.begin(); i != goalSet.end(); i++) {
        if (!(*i)->isSatisfied(gameState)) {
            return false;
        }
    }
    return true;
}


// Gets the heuristic for an action
// Takes the net average of all the heuristics from all the goals
void ConjunctiveGoalList::getActionHeuristic(State* newGameState, Action* act) {
    double sum = 0.0;
    for (list<Goal*>::iterator i = goalSet.begin(); i != goalSet.end(); i++) {
        sum += (*i)->getHeuristic(newGameState);
    }
    act->setHeuristic(sum / (double)goalSet.size());
}

#endif
