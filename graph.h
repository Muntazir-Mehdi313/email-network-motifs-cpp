
// Purpose: Defines graph structures and functions for network processing.

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>

using namespace std;

struct Graph {
    vector<int> nodes;
    vector<pair<int, int>> edges;
};

struct DepartmentInfo {
    int id;
    int internal_edges;
};

Graph load_edges(const string& filename);
vector<int> load_departments(const string& filename);
vector<DepartmentInfo> get_top_departments(
    const Graph& graph,
    const vector<int>& departments
);
Graph build_department_graph(
    const Graph& graph,
    const vector<int>& departments,
    int department_id
);

#endif
