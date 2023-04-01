#include <list>

#include "goal.h"
#include "state.h"
#include "action.h"

#ifndef goalList_H
#define goalList_H

using namespace std;

class GoalList {
  protected:
    list<Goal*> goalSet;

  public:
    GoalList() {};
    void addGoal(Goal* goal);
    void showGoals();
    bool isValid(State* gameState);
    virtual bool isSatisfied(State* gameState) = 0;
    virtual void getActionHeuristic(State* gameState, Action* act) = 0;
    ~GoalList();
};

// Push a goal new to the goal list
void GoalList::addGoal(Goal* goal) {
    goalSet.push_back(goal);
}


// Show all the goals in the list
void GoalList::showGoals() {
    for (list<Goal*>::iterator i = goalSet.begin(); i != goalSet.end(); i++) {
        (*i)->showHumanReadable();
    }
}


// Checks whether all the goals in the list are valid or not
bool GoalList::isValid(State* gameState) {
    for (list<Goal*>::iterator i = goalSet.begin(); i != goalSet.end(); i++) {
        if (!(*i)->isValid(gameState)) {
            return false;
        }
    }
    return true;
}


GoalList::~GoalList() {
    for (list<Goal*>::iterator i = goalSet.begin(); i != goalSet.end(); i++) {
        delete* i;
    }
}

#endif
