
// Purpose: Declares the directed edge-swap randomization function.

#ifndef RANDOMIZATION_H
#define RANDOMIZATION_H

#include <vector>
#include <utility>

using namespace std;

vector<pair<int, int>> randomize(
    const vector<pair<int, int>>& edges,
    int attempts,
    unsigned int seed
);

#endif
