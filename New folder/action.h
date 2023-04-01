#ifndef action_H
#define action_H


using namespace std;


class Action {
  int fromCol;
  int toCol;
  double heuristic;

  public:
    Action() {}; // Empty constructor (do not construct like this unless it is a placeholder)
    Action(int f, int t) : fromCol(f), toCol(t) {};
    Action(int f, int t, double hu) : fromCol(f), toCol(t), heuristic(hu) {};

    bool operator>(const Action& otherAct) const {return heuristic > otherAct.getHeuristic();};
    bool operator==(Action& otherAct);

    int getFromCol() {return fromCol;};
    int getToCol() {return toCol;}
    double getHeuristic() const {return heuristic;};
    void setHeuristic(double hu) {heuristic = hu;};
    bool isReverseOf(Action& otherAct);
    void show();
    void showHumanReadable();
};

// Equality operator
bool Action::operator==(Action& otherAct) {
    return fromCol == otherAct.getFromCol() && toCol == otherAct.getToCol();
}


// Prints out just the action tuple (to and from values)
void Action::show() {
    cout << "(" << fromCol << ", " << toCol << ")" << endl;
}


// Shows a human readable form for the user to read
void Action::showHumanReadable() {
    cout << "Moving from column " << fromCol << " to column " << toCol << endl;
}


// Checks if the action provided is the reverse of the current one
bool Action::isReverseOf(Action& otherAct) {
    return fromCol == otherAct.getToCol() && toCol == otherAct.getFromCol();
}
#endif
