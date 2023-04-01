/*********** Declaration*******
I hereby certify that no part of this assignment has been copied from
any other student’s work or from any other source. No part of the code
has been written/produced for me by another person or copied from any
other source.

I hold a copy of this assignment that I can produce if the original is
lost or damaged.
**************************/
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "state.h"
#include "action.h"
#include "goal.h"
#include "goalList.h"
#include "atomGoal.h"
#include "solver.h"
#include "neighbourGoal.h"
#include "disjunctiveGoalList.h"
#include "conjunctiveGoalList.h"


void manualInit(State* gameState);
void getBoardConfig(int& size, int& nums);
State* setupBoard();
GoalList* setupGoals(State* board);
void getGoalTuple(int& a, int& b, int& c);
void manualPlay();
void randomPlay();
void bestFirstPlay();


int main() {
  // Init random generator
  srand(time(NULL));

  int choice = 0;

  cout << "--- Mini SHRDLU by Marcus Belcastro (19185398) ---" << endl << endl;

  while (choice != 99) {
    cout << "Please select a game mode..." << endl;
    cout << "1. Manual game" << endl;
    cout << "2. Random game" << endl;
    cout << "3. AI game (best-first-search)" << endl;
    cout << "99. Exit" << endl;
    cout << "$ ";
    cin >> choice;

    switch (choice) {
      case 1:
        cout << "Starting manual game..." << endl << endl;
        manualPlay();
        break;
      case 2:
        cout << "Starting random game..." << endl << endl;
        randomPlay();
        break;
      case 3:
        bestFirstPlay();
        break;
      case 99:
        cout << "Thank you for playing." << endl << endl;
        break;
      default:
        cout << "Please select an option from the list." << endl << endl;
    }
  }

  return 0;
}


// Handles user input to formulate the game board in a custom state
void manualInit(State* gameState) {
  int input;
  for (int i = 1; i <= gameState->getNums(); i++) {
    gameState->showBoard();
    cout << endl << "Which column should " << i << " go to?" << endl;
    cout << "$ ";
    cin >> input;
    while (input < 0 || input >= gameState->getSize()) {
      cout << "Try again." << endl;
      cout << "$ ";
      cin >> input;
    }
    gameState->pushToCol(i, input);
  }
}


// Gets the configuration parameters for the board
// (size and number of tiles)
void getBoardConfig(int& size, int& nums) {
  while (size < 3) {
    cout << "What size is the board?" << endl;
    cout << "$ ";
    cin >> size;
  }

  while (size > nums || nums > size*size-size) {
    cout << "How many tiles are there?" << endl;
    cout << "$ ";
    cin >> nums;
  }
}


// Sets up everything State-related to set up the game board
// Returns a pointer to a state which must be deleted after use
State* setupBoard() {
  State* board;
  int choice = 0;
  int size=0, nums=0;

  while (choice < 1 || choice > 2) {
    cout << "Random board (1) or custom board (2)?" << endl;
    cout << "$ ";
    cin >> choice;
  }

  getBoardConfig(size, nums);
  board = new State(size, nums);

  if (choice == 2) {
    board->clearBoard();
    manualInit(board);
  }
  return board;
}


// This function builds a goal, either atom or neighbour by making the tuple
// It does not perform any checking and if the data is incorrect, it will
// be picked up in another function.
void getGoalTuple(int& a, int& b, int& c) {
  cout << "Provide the first number: ";
  cin >> a;
  cout << "*** Directions are (-1 : above, -2 : below, -3 : left, -4 : right) ***" << endl;
  cout << "*** If you chose a neighbour goal, select a direction. ***" << endl;
  cout << "Provide the row or direction: ";
  cin >> b;
  cout << "Provide the final column (or other number if neighbour goal): ";
  cin >> c;
}


// Sets up the goal list and returns a pointer to the newly formed goal list
// This pointer will need to be deleted when finished being used
GoalList* setupGoals(State* board) {
  GoalList* finalGoal;
  int choice = 0;

  while (choice < 1 || choice > 2) {
    cout << "Conjunctive goals (1) or Disjunctive goals (2)?" << endl;
    cout << "$ ";
    cin >> choice;
  }

  if (choice == 1) {
    finalGoal = new ConjunctiveGoalList();
  } else {
    finalGoal = new DisjunctiveGoalList();
  }

  while (choice != 99) {
    bool fail = false;
    Goal* currentGoal  = NULL;
    do {
      choice = 0;
      while ((choice < 1 || choice > 4) && choice != 99) {
        cout << "1. Manual atom goal" << endl;
        cout << "2. Manual neighbour goal" << endl;
        cout << "3. Auto atom goal" << endl;
        cout << "4. Auto neighbour goal" << endl;
        cout << "$ ";
        cin >> choice;
      }

      int a, b, c;

      switch (choice) {
        case 1:
          getGoalTuple(a, b, c);
          currentGoal = new AtomGoal(a, b, c);
          break;
        case 2:
          getGoalTuple(a, b, c);
          currentGoal = new NeighbourGoal(a, b, c);
          break;
        case 3:
          currentGoal = new AtomGoal(board);
          break;
        case 4:
          currentGoal = new NeighbourGoal(board);
          break;
      }

      fail = !currentGoal->isValid(board);
      if (fail) {
        cout << "Invalid Goal provided." << endl;
        delete currentGoal;
      } else {
        finalGoal->addGoal(currentGoal);
        cout << "Successfully added the goal: ";
        currentGoal->showHumanReadable();
      }
    } while (fail);

    choice = 0;
    while (choice != 1 && choice != 99) {
      cout << "Add another goal (1) or exit (99)?" << endl;
      cout << "$ ";
      cin >> choice;
    }
  }

  return finalGoal;
}


// Handle a manual game, take user input to perform actions
void manualPlay() {
  State* currentGame;
  GoalList* goal;

  // Specify the board
  currentGame = setupBoard();

  // Specify the goals
  goal = setupGoals(currentGame);

  currentGame->showBoard();
  while (!goal->isSatisfied(currentGame)) {
    Action currentAct;
    do {
      int a, b;
      cout << "From column: ";
      cin >> a;
      cout << "To column: ";
      cin >> b;
      currentAct = Action(a, b);
    } while (!currentGame->isValidAction(currentAct));

    currentGame->performAction(currentAct);
    currentGame->showBoard();
  }

  cout << "Congratulation you won!" << endl << endl;
  delete currentGame;
  delete goal;
}


// Play the game by making random actions
void randomPlay() {
  State* board;
  GoalList* goal;

  // Specify the board
  board = setupBoard();

  // Specify the goals
  goal = setupGoals(board);

  Solver currentGame = Solver(board, goal);

  board->showBoard();

  currentGame.randomSolver(1000);
}


// Play the game using best-first-search
void bestFirstPlay() {
  State* board;
  GoalList* goal;

  // Specify the board
  board = setupBoard();

  // Specify the goals
  goal = setupGoals(board);

  Solver currentGame = Solver(board, goal);

  // For tweaking the maximum recursion
  int maxSteps = 0;
  cout << "For smaller boards, lower max recursions work well. (Use max of 50)" << endl;
  cout << "How many max recursions?" << endl;
  while (maxSteps < 1) {
    cout << "$ ";
    cin >> maxSteps;
  }

  board->showBoard();

  currentGame.BFSSolver(maxSteps);
}
