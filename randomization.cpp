
// Purpose: Performs directed double-edge swaps and verifies degree preservation.

#include "randomization.h"
#include <set>
#include <map>
#include <random>
#include <cassert>

vector<pair<int, int>> randomize(
    const vector<pair<int, int>>& edges,
    int attempts,
    unsigned int seed
) {
    vector<pair<int, int>> result = edges;

    set<pair<int, int>> edge_set(
        result.begin(), result.end()
    );

    // Record original in-degree and out-degree.

    map<int, pair<int, int>> original_degrees;

    for (auto edge : edges) {
        original_degrees[edge.first].second++;
        original_degrees[edge.second].first++;
    }

    // Perform random edge-swap attempts.

    mt19937 generator(seed);

    if (result.size() >= 2) {
        uniform_int_distribution<int> distribution(
            0, (int)result.size() - 1
        );

        for (int i = 0; i < attempts; i++) {

            int first = distribution(generator);
            int second = distribution(generator);

            if (first == second) {
                continue;
            }

            int a = result[first].first;
            int b = result[first].second;

            int c = result[second].first;
            int d = result[second].second;

            // Reject self-loops and existing edges.

            if (a == d || c == b) {
                continue;
            }

            if (edge_set.count({a, d}) ||
                edge_set.count({c, b})) {
                continue;
            }

            // Replace the selected edges.

            edge_set.erase({a, b});
            edge_set.erase({c, d});

            edge_set.insert({a, d});
            edge_set.insert({c, b});

            result[first] = {a, d};
            result[second] = {c, b};
        }
    }

    // Verify that all degrees remain unchanged.

    map<int, pair<int, int>> new_degrees;

    for (auto edge : result) {
        new_degrees[edge.first].second++;
        new_degrees[edge.second].first++;
    }

    assert(original_degrees == new_degrees);
    assert(edge_set.size() == result.size());

    return result;
}
