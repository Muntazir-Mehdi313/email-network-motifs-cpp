
// Purpose: Classifies directed triads and counts all 13 connected triad classes.

#include "triads.h"
#include <algorithm>
#include <map>
#include <tuple>

int triad_class(
    int a,
    int b,
    int c,
    const set<pair<int, int>>& edges
) {
    array<int, 3> nodes = {a, b, c};

    sort(nodes.begin(), nodes.end());

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

    static const map<int, int> classes = {
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

array<long long, 13> count_triads(
    const vector<int>& nodes,
    const vector<pair<int, int>>& edges
) {
    array<long long, 13> counts = {};

    map<int, set<int>> neighbours;
    set<pair<int, int>> directed_edges;

    for (int node : nodes) {
        neighbours[node];
    }

    for (auto edge : edges) {
        int u = edge.first;
        int v = edge.second;

        directed_edges.insert(edge);

        neighbours[u].insert(v);
        neighbours[v].insert(u);
    }

    set<tuple<int, int, int>> unique_triads;

    for (auto entry : neighbours) {
        int center = entry.first;

        vector<int> adjacent(
            entry.second.begin(),
            entry.second.end()
        );

        for (int i = 0; i < (int)adjacent.size(); i++) {
            for (int j = i + 1; j < (int)adjacent.size(); j++) {
                int a = center;
                int b = adjacent[i];
                int c = adjacent[j];

                vector<int> triad = {a, b, c};

                sort(triad.begin(), triad.end());

                unique_triads.insert({
                    triad[0],
                    triad[1],
                    triad[2]
                });
            }
        }
    }

    for (auto triad : unique_triads) {
        int a = get<0>(triad);
        int b = get<1>(triad);
        int c = get<2>(triad);

        int class_number = triad_class(
            a, b, c, directed_edges
        );

        if (class_number >= 1 && class_number <= 13) {
            counts[class_number - 1]++;
        }
    }

    return counts;
}
