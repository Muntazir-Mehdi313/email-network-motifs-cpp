
// Purpose: Declares functions for classifying directed triads.

#ifndef TRIADS_H
#define TRIADS_H

#include <set>
#include <utility>

using namespace std;

int triad_class(
    int a,
    int b,
    int c,
    const set<pair<int, int>>& edges
);

#endif
