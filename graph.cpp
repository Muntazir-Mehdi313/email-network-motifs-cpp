
// Purpose: Loads and cleans the email graph, ranks departments and creates induced subgraphs.

#include "graph.h"
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <stdexcept>

Graph load_edges(const string& filename) {
    ifstream file(filename);

    if (!file) {
        throw runtime_error("Cannot open edge file.");
    }

    set<pair<int, int>> unique_edges;
    int u, v;

    while (file >> u >> v) {
        if (u != v) {
            unique_edges.insert({u, v});
        }
    }

    Graph graph;

    for (auto edge : unique_edges) {
        graph.edges.push_back(edge);
    }

    // Include all 1005 nodes, including isolated nodes.
    for (int i = 0; i < 1005; i++) {
        graph.nodes.push_back(i);
    }

    return graph;
}

vector<int> load_departments(const string& filename) {
    ifstream file(filename);

    if (!file) {
        throw runtime_error("Cannot open department file.");
    }

    vector<int> departments(1005, -1);

    int node, department;

    while (file >> node >> department) {
        if (node >= 0 && node < 1005) {
            departments[node] = department;
        }
    }

    return departments;
}

vector<DepartmentInfo> get_top_departments(
    const Graph& graph,
    const vector<int>& departments
) {
    vector<int> counts(42, 0);

    for (auto edge : graph.edges) {
        int u = edge.first;
        int v = edge.second;

        if (u >= 0 && u < 1005 &&
            v >= 0 && v < 1005 &&
            departments[u] >= 0 &&
            departments[u] < 42 &&
            departments[u] == departments[v]) {

            counts[departments[u]]++;
        }
    }

    vector<DepartmentInfo> ranking;

    for (int i = 0; i < 42; i++) {
        ranking.push_back({i, counts[i]});
    }

    sort(ranking.begin(), ranking.end(),
        [](DepartmentInfo a, DepartmentInfo b) {
            if (a.internal_edges != b.internal_edges) {
                return a.internal_edges > b.internal_edges;
            }

            return a.id < b.id;
        }
    );

    ranking.resize(20);

    return ranking;
}

Graph build_department_graph(
    const Graph& graph,
    const vector<int>& departments,
    int department_id
) {
    Graph subgraph;

    for (int i = 0; i < (int)departments.size(); i++) {
        if (departments[i] == department_id) {
            subgraph.nodes.push_back(i);
        }
    }

    set<int> members(subgraph.nodes.begin(), subgraph.nodes.end());

    for (auto edge : graph.edges) {
        if (members.count(edge.first) &&
            members.count(edge.second)) {

            subgraph.edges.push_back(edge);
        }
    }

    return subgraph;
}
