
// Purpose: Classifies directed triads using canonical 6-bit encoding.

#include "triads.h"
#include <algorithm>
#include <array>
#include <map>

int triad_class(
    int a,
    int b,
    int c,
    const set<pair<int, int>>& edges
) {
    array<int, 3> nodes = {a, b, c};

    int canonical_code = 63;

    do {
        int code = 0;

        if (edges.count({nodes[0], nodes[1]}))
            code |= (1 << 0);

        if (edges.count({nodes[0], nodes[2]}))
            code |= (1 << 1);

        if (edges.count({nodes[1], nodes[0]}))
            code |= (1 << 2);

        if (edges.count({nodes[1], nodes[2]}))
            code |= (1 << 3);

        if (edges.count({nodes[2], nodes[0]}))
            code |= (1 << 4);

        if (edges.count({nodes[2], nodes[1]}))
            code |= (1 << 5);

        canonical_code = min(canonical_code, code);

    } while (next_permutation(nodes.begin(), nodes.end()));

    map<int, int> classes = {
        {3, 1},
        {6, 2},
        {7, 3},
        {10, 4},
        {11, 5},
        {15, 6},
        {21, 7},
        {23, 8},
        {25, 9},
        {27, 10},
        {30, 11},
        {31, 12},
        {63, 13}
    };

    auto it = classes.find(canonical_code);

    if (it == classes.end()) {
        return 0;
    }

    return it->second;
}
