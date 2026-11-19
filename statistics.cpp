
// Purpose: Calculates triad means, population standard deviations and Z-scores.

#include "statistics.h"
#include <cmath>
#include <stdexcept>

array<Statistics, 13> calculate_statistics(
    const array<long long, 13>& observed,
    const vector<array<long long, 13>>& randomized
) {
    if (randomized.empty()) {
        throw runtime_error("No randomized networks available.");
    }

    array<Statistics, 13> results = {};

    int network_count = randomized.size();

    for (int i = 0; i < 13; i++) {

        double sum = 0;

        for (const auto& network : randomized) {
            sum += network[i];
        }

        double mean = sum / network_count;
        double variance = 0;

        for (const auto& network : randomized) {
            double difference = network[i] - mean;
            variance += difference * difference;
        }

        variance /= network_count;

        double standard_deviation = sqrt(variance);

        results[i].mean = mean;
        results[i].standard_deviation = standard_deviation;

        if (standard_deviation > 0) {
            results[i].z_score =
                (observed[i] - mean) / standard_deviation;

            results[i].valid = true;
        }
        else {
            results[i].z_score = 0;
            results[i].valid = false;
        }
    }

    return results;
}
