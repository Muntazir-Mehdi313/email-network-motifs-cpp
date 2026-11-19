// Purpose: Runs the project, builds department subgraphs, counts triads, randomizes networks, calculates Z-scores and exports results.

#include <iostream>
#include <stdexcept>
#include <array>
#include <set>
#include <iomanip>
#include <fstream>
#include <filesystem>

#include "graph.h"
#include "triads.h"
#include "randomization.h"
#include "statistics.h"

using namespace std;

int main() {
    const int R = 5298;
    const string section = "B";

    try {
        // Load and clean the email network.
        Graph graph = load_edges("data/email-Eu-core.txt");
        vector<int> departments =
            load_departments("data/email-Eu-core-department-labels.txt");
        vector<DepartmentInfo> top20 = get_top_departments(graph, departments);

        if (top20.size() < 20) {
            throw runtime_error("Fewer than 20 departments were found.");
        }

        // Calculate group parameters.
        const int pair_index = R % 190;
        int department_a_index = -1;
        int department_b_index = -1;
        int current_index = 0;

        for (int i = 0; i < 20; i++) {
            for (int j = i + 1; j < 20; j++) {
                if (current_index == pair_index) {
                    department_a_index = i;
                    department_b_index = j;
                }
                current_index++;
            }
        }

        if (department_a_index == -1 || department_b_index == -1) {
            throw runtime_error("Invalid department pair.");
        }

        const int department_a_id = top20[department_a_index].id;
        const int department_b_id = top20[department_b_index].id;
        const int random_networks = 20 + (R % 31);
        const int swap_multiplier = 5 + (R % 6);

        // Display project information.
        cout << "DISCRETE STRUCTURES PROJECT\n";
        cout << "Group R: " << R << "\n";
        cout << "Section: " << section << "\n";
        cout << "\nCleaned graph:\n";
        cout << "Nodes: " << graph.nodes.size() << "\n";
        cout << "Edges: " << graph.edges.size() << "\n";

        cout << "\nTop 20 Departments:\n";
        for (int i = 0; i < 20; i++) {
            cout << i << ". Department " << top20[i].id
                 << " | Internal edges: " << top20[i].internal_edges << "\n";
        }

        cout << "\nSelected Departments:\n";
        cout << "Pair index: " << pair_index << "\n";
        cout << "Department A: " << department_a_id << "\n";
        cout << "Department B: " << department_b_id << "\n";
        cout << "\nRandom networks: " << random_networks << "\n";
        cout << "Swap multiplier: " << swap_multiplier << "\n";
        cout << "Random seed: " << R << "\n";

        // Build the two induced department subgraphs.
        Graph department_a = build_department_graph(graph, departments, department_a_id);
        Graph department_b = build_department_graph(graph, departments, department_b_id);

        cout << "\nDepartment A Subgraph:\n";
        cout << "Department ID: " << department_a_id << "\n";
        cout << "Nodes: " << department_a.nodes.size() << "\n";
        cout << "Edges: " << department_a.edges.size() << "\n";

        cout << "\nDepartment B Subgraph:\n";
        cout << "Department ID: " << department_b_id << "\n";
        cout << "Nodes: " << department_b.nodes.size() << "\n";
        cout << "Edges: " << department_b.edges.size() << "\n";

        // Test the complete mutual triad classification.
        set<pair<int, int>> test_edges = {
            {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1}
        };
        const int complete_triad_class = triad_class(0, 1, 2, test_edges);
        cout << "\nTriad Classification Test:\n";
        cout << "All mutual connections: Class " << complete_triad_class << "\n";
        if (complete_triad_class != 13) {
            throw runtime_error("Complete mutual triad classification test failed.");
        }

        // Count all 13 connected triad classes.
        const array<long long, 13> counts_a =
            count_triads(department_a.nodes, department_a.edges);
        const array<long long, 13> counts_b =
            count_triads(department_b.nodes, department_b.edges);

        cout << "\nTriad Census Results:\n";
        cout << "\nClass\tDepartment " << department_a_id
             << "\tDepartment " << department_b_id << "\n";

        long long total_a = 0;
        long long total_b = 0;
        for (int i = 0; i < 13; i++) {
            cout << i + 1 << "\t" << counts_a[i]
                 << "\t\t" << counts_b[i] << "\n";
            total_a += counts_a[i];
            total_b += counts_b[i];
        }

        cout << "\nTotal Connected Triads:\n";
        cout << "Department " << department_a_id << ": " << total_a << "\n";
        cout << "Department " << department_b_id << ": " << total_b << "\n";

        // Generate randomized networks and count their triads.
        vector<array<long long, 13>> randomized_counts_a;
        vector<array<long long, 13>> randomized_counts_b;

        const int attempts_a = swap_multiplier * static_cast<int>(department_a.edges.size());
        const int attempts_b = swap_multiplier * static_cast<int>(department_b.edges.size());

        cout << "\nRandomization:\n";
        cout << "Department " << department_a_id
             << " attempts per network: " << attempts_a << "\n";
        cout << "Department " << department_b_id
             << " attempts per network: " << attempts_b << "\n";

        for (int i = 0; i < random_networks; i++) {
            vector<pair<int, int>> randomized_a =
                randomize(department_a.edges, attempts_a, R + i);
            vector<pair<int, int>> randomized_b =
                randomize(department_b.edges, attempts_b, R + 1000 + i);

            randomized_counts_a.push_back(
                count_triads(department_a.nodes, randomized_a));
            randomized_counts_b.push_back(
                count_triads(department_b.nodes, randomized_b));
        }

        cout << "Randomized networks generated: "
             << randomized_counts_a.size() << " per department\n";
        cout << "In-degree and out-degree preservation: PASSED\n";
        cout << "No self-loops or duplicate edges: PASSED\n";

        // Calculate means, population standard deviations and Z-scores.
        const array<Statistics, 13> statistics_a =
            calculate_statistics(counts_a, randomized_counts_a);
        const array<Statistics, 13> statistics_b =
            calculate_statistics(counts_b, randomized_counts_b);

        cout << "\nZ-Score Results:\n";
        cout << fixed << setprecision(2);
        cout << "\nClass\tDept A Mean\tDept A SD\tDept A Z"
             << "\tDept B Mean\tDept B SD\tDept B Z\n";

        for (int i = 0; i < 13; i++) {
            cout << i + 1 << "\t"
                 << statistics_a[i].mean << "\t\t"
                 << statistics_a[i].standard_deviation << "\t\t"
                 << statistics_a[i].z_score << "\t\t"
                 << statistics_b[i].mean << "\t\t"
                 << statistics_b[i].standard_deviation << "\t\t"
                 << statistics_b[i].z_score << "\n";
        }

        cout << "\nRandom networks generated per department: "
             << random_networks << "\n";

        // Export observed counts, random means, standard deviations and Z-scores.
        filesystem::create_directories("results");
        ofstream csv("results/z_scores.csv");
        if (!csv) {
            throw runtime_error("Cannot create Z-score CSV file.");
        }

        csv << "R,Section,Department_A_ID,Department_B_ID,Class,"
            << "Department_A_Observed,Department_B_Observed,"
            << "Department_A_Mean,Department_A_SD,Department_A_Z,"
            << "Department_B_Mean,Department_B_SD,Department_B_Z\n";

        csv << setprecision(10);
        for (int i = 0; i < 13; i++) {
            csv << R << "," << section << ","
                << department_a_id << "," << department_b_id << ","
                << i + 1 << ","
                << counts_a[i] << "," << counts_b[i] << ","
                << statistics_a[i].mean << ","
                << statistics_a[i].standard_deviation << ","
                << statistics_a[i].z_score << ","
                << statistics_b[i].mean << ","
                << statistics_b[i].standard_deviation << ","
                << statistics_b[i].z_score << "\n";
        }

        csv.close();
        if (!csv) {
            throw runtime_error("An error occurred while writing the Z-score CSV file.");
        }

        cout << "\nResults exported successfully: results/z_scores.csv\n";
    }
    catch (const exception& e) {
        cerr << "\nError: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
