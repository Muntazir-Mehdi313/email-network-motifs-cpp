// Purpose: Performs directed double-edge swaps and verifies degree preservation.

#include "randomization.h"
#include <set>
#include <map>
#include <random>
#include <cassert>
#include <stdexcept>

namespace {

using DegreeMap = map<int, pair<int, int>>;

DegreeMap calculate_degrees(const vector<pair<int, int>>& edges) {
    DegreeMap degrees;

    for (const auto& edge : edges) {
        degrees[edge.first].second++;
        degrees[edge.second].first++;
    }

    return degrees;
}

} // namespace

vector<pair<int, int>> randomize(
    const vector<pair<int, int>>& edges,
    int attempts,
    unsigned int seed
) {
    if (attempts < 0) {
        throw invalid_argument("Swap attempts cannot be negative.");
    }

    vector<pair<int, int>> result = edges;
    set<pair<int, int>> edge_set(result.begin(), result.end());

    if (edge_set.size() != result.size()) {
        throw invalid_argument("Input graph contains duplicate directed edges.");
    }

    const DegreeMap original_degrees = calculate_degrees(edges);
    mt19937 generator(seed);

    if (result.size() >= 2) {
        uniform_int_distribution<int> distribution(
            0, static_cast<int>(result.size()) - 1
        );

        for (int i = 0; i < attempts; i++) {
            const int first = distribution(generator);
            const int second = distribution(generator);

            if (first == second) {
                continue;
            }

            const int a = result[first].first;
            const int b = result[first].second;
            const int c = result[second].first;
            const int d = result[second].second;

            // Reject self-loops and edges that would already exist.
            if (a == d || c == b) {
                continue;
            }

            if (edge_set.count({a, d}) || edge_set.count({c, b})) {
                continue;
            }

            edge_set.erase({a, b});
            edge_set.erase({c, d});

            edge_set.insert({a, d});
            edge_set.insert({c, b});

            result[first] = {a, d};
            result[second] = {c, b};
        }
    }

    const DegreeMap randomized_degrees = calculate_degrees(result);

    // Assertions are retained for the assignment; runtime checks also work
    // when the program is compiled with NDEBUG.
    assert(original_degrees == randomized_degrees);
    assert(edge_set.size() == result.size());

    if (original_degrees != randomized_degrees) {
        throw logic_error("Randomization changed an in-degree or out-degree.");
    }

    if (edge_set.size() != result.size()) {
        throw logic_error("Randomization created duplicate directed edges.");
    }

    for (const auto& edge : result) {
        if (edge.first == edge.second) {
            throw logic_error("Randomization created a self-loop.");
        }
    }

    return result;
}
