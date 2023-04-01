#include <unordered_set>
#include <string>
#include <vector>
#include <list>
#include <queue>

#include "state.h"
#include "action.h"
#include "goalList.h"
#include "randomness.h"

using namespace std;

#ifndef solver_H
#define solver_H


class Solver {
  list<Action> plan;
  State* mainState;
  GoalList* finalGoal;
  unordered_set<string> hashSet;

  public:
    // Garbage collection of mainState and finalGoal is handled in destructor
    Solver(State* s, GoalList* g) : mainState(s), finalGoal(g) {};
    void addToPlan(Action act);
    void printPlan();
    bool hashExists(string hash);
    bool bestFirstSearch(State* node, int maxRecurse);
    void getHeuristicActions(
      State* currentState,
      priority_queue<Action, vector<Action>, greater<Action>>& q
    );

    void randomSolver(int maxSteps=100);
    void BFSSolver(int maxRecurse=100);

    ~Solver();
};

// The random solver solves the game by picking a random action that is
// not the reverse of the current action, hence reducing loops
void Solver::randomSolver(int maxSteps) {
    Action prevAct;
    int levels = 0;
    // While we still have steps we can make and the board is not solved
    while (levels < maxSteps && !finalGoal->isSatisfied(mainState)) {
        // Sore all the current moves for the level of the tree
        vector<Action> currentLevel;
        mainState->getPossibleMoves(currentLevel);
        Action choice = currentLevel.at(getRand(0, currentLevel.size() - 1));
        // While the action is the reverse of the current action, get a new action
        while (choice.isReverseOf(prevAct)) {
            choice = currentLevel.at(getRand(0, currentLevel.size() - 1));
        }
        // Perform the action
        mainState->performAction(choice);
        // Store it as the previous action
        prevAct = choice;
        // Add it to the plan
        addToPlan(choice);
        levels++;
    }

    // Print everything if satisfied or not
    if (finalGoal->isSatisfied(mainState)) {
        cout << "We found a solution using random actions! Printing the plan..." << endl;
        printPlan();
        mainState->showBoard();
    }
    else {
        cout << "No solution found :(" << endl;
    }
}


// The base function that begins executing the recursion and prints success
void Solver::BFSSolver(int maxRecurse) {
    // Put the root node in the hash set
    hashExists(mainState->getHash());
    if (bestFirstSearch(mainState, maxRecurse)) {
        cout << "We found a solution using Best-first-search! Printing the plan..." << endl;
        printPlan();
        mainState->showBoard();
    }
    else {
        cout << "No solution found :(" << endl;
    }
}


// The true recursive best first search algorithm
// `node` is the currently analysed state in the tree (the root node for the context)
// `maxRecurse` limits the number of recursions to be memory safe
bool Solver::bestFirstSearch(State* node, int maxRecurse) {
    // If at the end of the recursion, terminate with false
    if (maxRecurse < 1) {
        return false;
    }
    // Get the ordered queue of the possible actions
    priority_queue<Action, vector<Action>, greater<Action>> nextActions;
    // Assign the heuristics to them
    getHeuristicActions(node, nextActions);

    // Iterate through the ordered actions
    while (!nextActions.empty()) {
        // Store the best action
        Action nextAct = nextActions.top();
        State* nextNode = new State(node); // Copy the current node
        nextNode->performAction(nextAct); // Perform the action
        addToPlan(nextAct); // Add it to the plan before recursing

        // If this action leads to a winning board, say that it was found
        if (finalGoal->isSatisfied(nextNode)) {
            delete mainState; // Free up the old mainState
            // Set the winning board to be the main state
            mainState = nextNode;
            return true;
            // Else recurse and catch any found solutions
        }
        else if (bestFirstSearch(nextNode, maxRecurse - 1)) {
            delete nextNode; // Free up any memory used
            return true;
        }
        // Else the action was unsuccessful, delete unecessary data
        delete nextNode;
        plan.pop_back(); // Get rid of the failed action
        nextActions.pop(); // move to the next action
    }
    return false;
}


// Gets the priority queue of all the actions for a current state
// This function is needed to filter out duplicate states and
// to convert a vector into a priority queue
void Solver::getHeuristicActions(
    State* currentState,
    priority_queue<Action, vector<Action>, greater<Action>>& q
) {
    vector<Action> allActs;
    currentState->getPossibleMoves(allActs);
    for (vector<Action>::iterator i = allActs.begin(); i != allActs.end(); i++) {
        // Perform the action so the state can be analysed
        currentState->performAction(*i);
        // If the hash of the state doesn't exist, then check the heuristic
        if (!hashExists(currentState->getHash())) {
            finalGoal->getActionHeuristic(currentState, &(*i));
            // After the heuristic is added to the action, load it into the queue
            q.push(*i);
        }
        // Reverse the current state for the next action to test
        currentState->reverseAction(*i);
    }
}


// Automatically store nonexistent hashes and return whether they already exist
// This is used to prune the tree from duplicates and reverse states and actions
bool Solver::hashExists(string hash) {
    if (hashSet.count(hash) > 0) {
        return true;
    }
    else {
        hashSet.insert(hash);
        return false;
    }
}


void Solver::addToPlan(Action act) {
    plan.push_back(act);
}


void Solver::printPlan() {
    for (list<Action>::iterator i = plan.begin(); i != plan.end(); i++) {
        i->showHumanReadable();
    }
}


Solver::~Solver() {
    delete mainState;
    delete finalGoal;
}


#endif
