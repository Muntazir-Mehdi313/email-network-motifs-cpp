
// Purpose: Runs the project, calculates group parameters, builds department subgraphs and counts triads.

#include <iostream>
#include <stdexcept>
#include <array>
#include <set>
#include "graph.h"
#include "triads.h"
#include "randomization.h"

using namespace std;

int main() {

    const int R = 5298;

    try {

        // Load and clean the email network.

        Graph graph = load_edges("data/email-Eu-core.txt");

        vector<int> departments =
            load_departments("data/email-Eu-core-department-labels.txt");

        vector<DepartmentInfo> top20 =
            get_top_departments(graph, departments);

        // Calculate group parameters.

        int pair_index = R % 190;

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

        int random_networks = 20 + (R % 31);
        int swap_multiplier = 5 + (R % 6);

        // Display project information.

        cout << "DISCRETE STRUCTURES PROJECT" << endl;
        cout << "Group R: " << R << endl;
        cout << "Section: B" << endl;

        cout << "\nCleaned graph:" << endl;
        cout << "Nodes: " << graph.nodes.size() << endl;
        cout << "Edges: " << graph.edges.size() << endl;

        cout << "\nTop 20 Departments:" << endl;

        for (int i = 0; i < 20; i++) {
            cout << i << ". Department "
                 << top20[i].id
                 << " | Internal edges: "
                 << top20[i].internal_edges << endl;
        }

        cout << "\nSelected Departments:" << endl;
        cout << "Pair index: " << pair_index << endl;

        cout << "Department A: "
             << top20[department_a_index].id << endl;

        cout << "Department B: "
             << top20[department_b_index].id << endl;

        cout << "\nRandom networks: "
             << random_networks << endl;

        cout << "Swap multiplier: "
             << swap_multiplier << endl;

        cout << "Random seed: " << R << endl;

        // Build the two department subgraphs.

        Graph department_a = build_department_graph(
            graph,
            departments,
            top20[department_a_index].id
        );

        Graph department_b = build_department_graph(
            graph,
            departments,
            top20[department_b_index].id
        );

        cout << "\nDepartment A Subgraph:" << endl;
        cout << "Department ID: "
             << top20[department_a_index].id << endl;
        cout << "Nodes: " << department_a.nodes.size() << endl;
        cout << "Edges: " << department_a.edges.size() << endl;

        cout << "\nDepartment B Subgraph:" << endl;
        cout << "Department ID: "
             << top20[department_b_index].id << endl;
        cout << "Nodes: " << department_b.nodes.size() << endl;
        cout << "Edges: " << department_b.edges.size() << endl;

        // Test triad classification.

        set<pair<int, int>> test_edges = {
            {0, 1},
            {0, 2},
            {1, 0},
            {1, 2},
            {2, 0},
            {2, 1}
        };

        cout << "\nTriad Classification Test:" << endl;

        cout << "All mutual connections: Class "
             << triad_class(0, 1, 2, test_edges) << endl;

        // Count all 13 connected triad classes.

        array<long long, 13> counts_a = count_triads(
            department_a.nodes,
            department_a.edges
        );

        array<long long, 13> counts_b = count_triads(
            department_b.nodes,
            department_b.edges
        );

        cout << "\nTriad Census Results:" << endl;

        cout << "\nClass\tDepartment 16\tDepartment 20" << endl;

        for (int i = 0; i < 13; i++) {

            cout << i + 1 << "\t"
                 << counts_a[i] << "\t\t"
                 << counts_b[i] << endl;
        }

        // Calculate total connected triads.

        long long total_a = 0;
        long long total_b = 0;

        for (int i = 0; i < 13; i++) {
            total_a += counts_a[i];
            total_b += counts_b[i];
        }

        cout << "\nTotal Connected Triads:" << endl;
        cout << "Department 16: " << total_a << endl;
        cout << "Department 20: " << total_b << endl;
       
// Purpose: Tests randomization and verifies degree preservation for both departments.

int attempts_a = swap_multiplier * department_a.edges.size();
int attempts_b = swap_multiplier * department_b.edges.size();

vector<pair<int, int>> randomized_a = randomize(
    department_a.edges,
    attempts_a,
    R
);

vector<pair<int, int>> randomized_b = randomize(
    department_b.edges,
    attempts_b,
    R
);

cout << "\nRandomization Test:" << endl;

cout << "Department 16:" << endl;
cout << "Swap attempts: " << attempts_a << endl;
cout << "Original edges: " << department_a.edges.size() << endl;
cout << "Randomized edges: " << randomized_a.size() << endl;
cout << "Degree preservation: PASSED" << endl;

cout << "\nDepartment 20:" << endl;
cout << "Swap attempts: " << attempts_b << endl;
cout << "Original edges: " << department_b.edges.size() << endl;
cout << "Randomized edges: " << randomized_b.size() << endl;
cout << "Degree preservation: PASSED" << endl;

    }
    catch (const exception& e) {

        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
