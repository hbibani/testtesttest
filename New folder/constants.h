/*********** Declaration*******
I hereby certify that no part of this assignment has been copied from
any other student’s work or from any other source. No part of the code
has been written/produced for me by another person or copied from any
other source.

I hold a copy of this assignment that I can produce if the original is
lost or damaged.
**************************/
#include <string>

using namespace std;

#ifndef constants_H
#define constants_H

const int DEFAULT_SIZE = 3;
const int DEFAULT_NUMS = 6;

// Directions are encoded as negatives to allow for least complex processing
const int ABOVE = -1;
const int BELOW = -2;
const int LEFT = -3;
const int RIGHT = -4;

// List so that random functions can pick easily
const int DIRECTIONS[] = {ABOVE, BELOW, LEFT, RIGHT};
// Add 4 to the direction and then use it as an index to get the character
const char DIRECTION_CHARS[] = {'r', 'l', 'b', 'a'};
// Add 4 to the direction and then use it as an index to get the string
const string DIRECTION_STRS[] = {"right of", "left of", "below", "above"};


#endif
