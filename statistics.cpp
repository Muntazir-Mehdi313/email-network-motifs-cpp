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
    const double network_count = static_cast<double>(randomized.size());

    for (int i = 0; i < 13; i++) {
        double sum = 0.0;

        for (const auto& network : randomized) {
            sum += static_cast<double>(network[i]);
        }

        const double mean = sum / network_count;
        double variance = 0.0;

        for (const auto& network : randomized) {
            const double difference = static_cast<double>(network[i]) - mean;
            variance += difference * difference;
        }

        // Population standard deviation is used for the randomized sample.
        variance /= network_count;
        const double standard_deviation = sqrt(variance);

        results[i].mean = mean;
        results[i].standard_deviation = standard_deviation;

        // The project specification defines Z = 0 when SD = 0.
        if (standard_deviation == 0.0) {
            results[i].z_score = 0.0;
        }
        else {
            results[i].z_score =
                (static_cast<double>(observed[i]) - mean) / standard_deviation;
        }

        results[i].valid = true;
    }

    return results;
}
