#ifndef randomness_H
#define randomness_H

using namespace std;

int getRand(int a, int b);
// Get a random number between two positive integers a and b (inclusive)
int getRand(int a, int b) {
	return rand() % (b - a + 1) + a;
}
#endif
