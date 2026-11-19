
// Purpose: Starts the project, loads the email network and calculates group parameters.

#include <iostream>
#include <stdexcept>
#include "graph.h"

using namespace std;

int main() {
    const int R = 5298;

    try {
        Graph graph = load_edges("data/email-Eu-core.txt");

        vector<int> departments =
            load_departments("data/email-Eu-core-department-labels.txt");

        vector<DepartmentInfo> top20 =
            get_top_departments(graph, departments);

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

        int random_networks = 20 + (R % 31);
        int swap_multiplier = 5 + (R % 6);

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

// Purpose: Builds and displays the two selected department subgraphs.

Graph department_a = build_department_graph(
    graph, departments, top20[department_a_index].id
);

Graph department_b = build_department_graph(
    graph, departments, top20[department_b_index].id
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

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
