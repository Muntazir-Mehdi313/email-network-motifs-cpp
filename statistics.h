
// Purpose: Declares statistical functions for triad analysis.

#ifndef STATISTICS_H
#define STATISTICS_H

#include <array>
#include <vector>

using namespace std;

struct Statistics {
    double mean;
    double standard_deviation;
    double z_score;
    bool valid;
};

array<Statistics, 13> calculate_statistics(
    const array<long long, 13>& observed,
    const vector<array<long long, 13>>& randomized
);

#endif
