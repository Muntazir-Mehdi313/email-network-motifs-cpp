
// Purpose: Declares functions for triad classification and counting.

#ifndef TRIADS_H
#define TRIADS_H

#include <set>
#include <vector>
#include <array>
#include <utility>

using namespace std;

int triad_class(
    int a,
    int b,
    int c,
    const set<pair<int, int>>& edges
);

array<long long, 13> count_triads(
    const vector<int>& nodes,
    const vector<pair<int, int>>& edges
);

#endif
