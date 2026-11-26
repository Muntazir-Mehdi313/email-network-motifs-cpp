#include "statistics.h"
#include <cmath>

void calculateStatistics(
    int observed[13],
    int randomized[][13],
    int numberOfRandomized,
    Statistics results[13]
)
{
    for (int i = 0; i < 13; i++)
    {
        double sum = 0;

        // Calculate mean
        for (int j = 0; j < numberOfRandomized; j++)
        {
            sum += randomized[j][i];
        }

        double mean = sum / numberOfRandomized;

        // Calculate variance
        double variance = 0;

        for (int j = 0; j < numberOfRandomized; j++)
        {
            double difference = randomized[j][i] - mean;

            variance += difference * difference;
        }

        variance = variance / numberOfRandomized;

        // Calculate standard deviation
        double standardDeviation = sqrt(variance);

        results[i].mean = mean;
        results[i].standard_deviation = standardDeviation;

        // Calculate Z-score
        if (standardDeviation == 0)
        {
            results[i].z_score = 0;
        }
        else
        {
            results[i].z_score =
                (observed[i] - mean) / standardDeviation;
        }

        results[i].valid = true;
    }
}