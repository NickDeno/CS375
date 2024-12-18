#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <algorithm>
#include <chrono>
#include <queue>
using namespace std;
using namespace std::chrono;

//2.
bool isCyclicUtil(vector<vector<int>>& adj, int currNode, vector<bool>& visited, vector<bool>& recStack) {
    if (!visited[currNode]) {
        visited[currNode] = true;
        recStack[currNode] = true;
        for (int v : adj[currNode]) {
            if (!visited[v] && 
                isCyclicUtil(adj, v, visited, recStack))
                return true;
            else if (recStack[v])
                return true;
        }
    }
    recStack[currNode] = false;
    return false;
}

//2.
bool isCyclic(vector<vector<int>>& adj, int n) {
    vector<bool> visited(n, false);
    vector<bool> recStack(n, false);
    for (int i = 0; i < n; i++) {
        if (!visited[i] && isCyclicUtil(adj, i, visited, recStack))
            return true;
    }
    return false;
}

//5.
void dfs(int node, unordered_map<int, vector<int>>& graph, unordered_map<int, bool>& visited,
         vector<pair<int, int>>& discovery, stack<pair<int, int>>& finishing,
         int& cost, unordered_map<int, unordered_map<int, char>>& edge_types, int parent) {
    
    visited[node] = true;
    cost++;
    discovery.push_back({node, cost}); // Add node and discovery cost
    for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            edge_types[node][neighbor] = 'T';  // Tree Edge
            dfs(neighbor, graph, visited, discovery, finishing, cost, edge_types, node);
        } else if (neighbor == parent) {
            continue;
        } else if (discovery[neighbor].second < discovery.back().second) {
            edge_types[node][neighbor] = 'B';  // Backward Edge
        } else if (discovery[neighbor].second > discovery.back().second) {
            edge_types[node][neighbor] = 'F';  // Forward Edge
        } else {
            edge_types[node][neighbor] = 'C';  // Cross Edge
        }
    }
    cost++;
    finishing.push({node, cost});  // Add finishing cost to front of finishing stack
}

//5.
void topologicalSort(unordered_map<int, vector<int>>& graph) {
    unordered_map<int, bool> visited;
    vector<pair<int, int>> discovery;
    stack<pair<int, int>> finishing;
    unordered_map<int, unordered_map<int, char>> edge_types; 
    int cost = 0;

    //Start DFS with node 1
    dfs(1, graph, visited, discovery, finishing, cost, edge_types, -1);
    // Visit remaining unvisited nodes (to handle disconnected components)
    for (const auto& pair : graph) {
        int node = pair.first;
        if (!visited[node]) {
            dfs(node, graph, visited, discovery, finishing, cost, edge_types, -1);
        }
    }

    cout << endl;
    // Print discovery costs
    cout << "Discovery order/costs: ";
    for (const auto& pair : discovery) {
        cout << pair.first << ":" << pair.second << ", ";
    }
    cout << endl;
    // Print finishing costs
    cout << "Finishing order/costs (Also topological order): ";
    while (!finishing.empty()) {
        auto node = finishing.top();
        finishing.pop();
        cout << node.first << ":" << node.second << ", ";
    }
    cout << endl;
    // Print edge types
    cout << "Edge Types: " << endl;
    for (const auto& node : edge_types) {
        for (const auto& neighbor : node.second) {
            cout << "Edge (" << node.first << " -> " << neighbor.first << "): " << neighbor.second << endl;
        }
    }
}

//6. Extra Credit
bool isBipartite(const vector<vector<int>>& adj, vector<int>& V1, vector<int>& V2) {
    vector<int> color(adj.size(), -1); // -1 indicates uncolored
    queue<int> q;

    for (int i = 0; i < adj.size(); i++) {
        // If the vertex is uncolored, perform BFS
        if (color[i] == -1) {
            color[i] = 0; // Start coloring with color 0 (first color)
            q.push(i);
            V1.push_back(i); // Add to V1

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                // Check all adjacent vertices
                for (int v : adj[u]) {
                    if (color[v] == -1) {
                        // Color the neighbor with the opposite color
                        color[v] = 1 - color[u];
                        if (color[v] == 0) {
                            V1.push_back(v);
                        } else {
                            V2.push_back(v);
                        }
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        // If the neighbor has the same color, it's not bipartite
                        return false;
                    }
                }
            }
        }
    }
    return true; // Successfully colored the graph
}

//B1.
int minimumSemesters(int n, vector<pair<int, int>>& prerequisites) {
    vector<vector<int>> adj(n);
    vector<int> prerequisiteCount(n, 0); // Number of prerequisites for each course
    vector<int> semester(n, 0); // To track which semester a course can be taken

    // Build the graph from the prereqs
    for (auto& pre : prerequisites) {
        int u = pre.first;
        int v = pre.second;
        adj[u-1].push_back(v-1);
        prerequisiteCount[v-1]++;
    }

    // Initialize the queue with courses that have no prerequisites
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (prerequisiteCount[i] == 0) {
            q.push(i);
            semester[i] = 0; // Can be taken in the first semester
        }
    }

    // Process the courses semester by semester
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (int neighbor : adj[current]) {
            //Decrements prereq count for negihboring courses
            //If neighboring prereq count = 0, we add it to the queue
            prerequisiteCount[neighbor]--;
            if (prerequisiteCount[neighbor] == 0) {
                q.push(neighbor);
                semester[neighbor] = semester[current] + 1; // Increment semester when ready
            }
        }
    }

    // Return the maximum semester number
    int maxSemester = 0;
    for (int i = 0; i < n; i++) {
        maxSemester = max(maxSemester, semester[i]);
    }
    // 0 indexed, so first semester = 0, second semester = 1...., if we want 1 indexed semesters we add 1
    return maxSemester+1;
}

//B2.
void printTableau(const vector<vector<double>>& tableau) {
    cout << "\nCurrent Tableau:\n";
    for (const auto& row : tableau) {
        for (double element : row)
            cout << element << "\t";
        cout << endl;
    }
}

//B2.
void simplexAlgorithm(vector<vector<double>>& tableau) {
    // Number of rows
    int m = tableau.size();
    // Number of columns
    int n = tableau[0].size();
    printTableau(tableau);
    while (true) {
        // Find pivot column (most negative element in last row)
        int pivot_col = -1;
        double min_value = 0.0;
        for (int j = 0; j < n - 1; ++j) {
            if (tableau[m - 1][j] < min_value) {
                min_value = tableau[m - 1][j];
                pivot_col = j;
            }
        }
        // If there are no negative values in the last row, the current solution is optimal
        if (pivot_col == -1)
            break;

        // Find the pivot row using ratios (Dividie each 
        // element in right-most column by pivot row element )
        int pivot_row = -1;
        double min_ratio = numeric_limits<double>::infinity();
        for (int i = 0; i < m - 1; ++i) {
            if (tableau[i][pivot_col] > 0) {
                double ratio = tableau[i][n - 1] / tableau[i][pivot_col];
                if (ratio < min_ratio) {
                    min_ratio = ratio;
                    pivot_row = i;
                }
            }
        }
        // If no pivot row found, => solution is unbounded
        if (pivot_row == -1) {
            return;
        }

        // Perform row operations, Make pivot value 1 and zero out pivot column
        double pivot_value = tableau[pivot_row][pivot_col];
        for (int j = 0; j < n; ++j)
            tableau[pivot_row][j] /= pivot_value;
        for (int i = 0; i < m; ++i) {
            if (i != pivot_row) {
                double multiplier = tableau[i][pivot_col];
                for (int j = 0; j < n; ++j)
                    tableau[i][j] -= multiplier * tableau[pivot_row][j];
            }
        }
        printTableau(tableau);
    }
}

int main(int argc, char* argv[]) {
    //2.
    // Example graph:
    // 0 -> 1, 1 -> 2, 2 -> 3, 3 -> 4, 4 -> 2, 4 -> 5
    // 4 -> 2 introduces a cycle (4 -> 2 -> 3 -> 4)
    vector<vector<int>> adj1(6);
    adj1[0].push_back(1);
    adj1[1].push_back(2);
    adj1[2].push_back(3);
    adj1[3].push_back(4);
    adj1[4].push_back(2);
    adj1[4].push_back(5);
    auto start = high_resolution_clock::now();
    if (isCyclic(adj1, 6)) {
        cout << "Graph contains a cycle" << endl;
    } else {
        cout << "Graph does not contain a cycle" << endl;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Running time of isCyclic: " << duration.count() << " microseconds" << endl;

    //5.
    unordered_map<int, vector<int>> adj2;
    adj2[1] = {7, 2, 4};
    adj2[7] = {3};
    adj2[3] = {};
    adj2[2] = {3, 4, 7};
    adj2[4] = {8};
    adj2[8] = {};
    adj2[6] = {2, 4, 5};
    adj2[5] = {2, 3};
    start = high_resolution_clock::now();
    topologicalSort(adj2);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Running time of Topoligical Sort: " << duration.count() << " microseconds\n" << endl;

    //6. Extra Credit
    vector<vector<int>> adj3(4);
    vector<int> V1, V2;
    adj3[0].push_back(1); adj3[1].push_back(0); 
    adj3[1].push_back(2); adj3[2].push_back(1);  
    adj3[2].push_back(3); adj3[3].push_back(2);  
    adj3[3].push_back(0); adj3[0].push_back(3);
    if (isBipartite(adj3,V1, V2)) {
        cout << "The graph is bipartite." << endl;
        cout << "Set V1: { ";
        for (int node : V1) {
            cout << node << " ";
        }
        cout << "}" << endl;

        cout << "Set V2: { ";
        for (int node : V2) {
            cout << node << " ";
        }
        cout << "}\n" << endl;
    } else {
        cout << "The graph is not bipartite.\n" << endl;
    }

    //B1.
    vector<pair<int, int>> prerequisites = {
        {2, 4}, {4, 5}, {1, 6}, {3, 6}, {4, 7}, {5, 8}, {6, 8}, {7, 9},
        {9, 10}, {8, 11}, {3, 12}, {6, 13}, {4, 14}, {6, 14}, {14, 15}
    };
    start = high_resolution_clock::now();
    int result = minimumSemesters(15, prerequisites);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Minimum number of semesters needed: " << result << endl;
    cout << "Running time of minimumSemester: " << duration.count() << " microseconds" << endl;

    //B2.
    vector<vector<double>> tableau = {
        {60, 60, 1, 0, 0, 300},
        {12, 6,  0, 1, 0, 36},
        {10, 30, 0, 0, 1, 90},
        {-.12, -.15, 0, 0, 0, 0}
    };
    start = high_resolution_clock::now();
    simplexAlgorithm(tableau);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Amount of Drink X consumed: " << tableau[1][tableau[0].size() - 1] << endl;
    cout << "Amount of Drink Y consumed: " << tableau[2][tableau[0].size() - 1] << endl;
    cout << "Min cost(P): " << tableau[tableau.size() - 1][tableau[0].size() - 1] << endl;
    cout << "Running time of Simplex Tableau 1: " << duration.count() << " microseconds" << endl;

    //B3 (Extra Credit)
    vector<vector<double>> tableau2 = {
        {3, 2, 5, 1, 0, 0, 0, 55},  // First constraint with slack variable s1
        {2, 1, 1, 0, 1, 0, 0, 26},  // Second constraint with slack variable s2
        {1, 1, 3, 0, 0, 1, 0, 30},  // Third constraint with slack variable s3
        {5, 2, 4, 0, 0, 0, 1, 57},  // Fourth constraint with slack variable s4
        {-20, -10, -15, 0, 0, 0, 0, 0}  // Objective function: Max P = 20x1 + 10x2 + 15x3
    };
    start = high_resolution_clock::now();
    simplexAlgorithm(tableau2);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "X1: " << tableau2[3][tableau2[0].size() - 1] << endl;
    cout << "X2: " << tableau2[1][tableau2[0].size() - 1] << endl;
    cout << "X3: " << tableau2[0][tableau2[0].size() - 1] << endl;
    cout << "Max cost(P): " << tableau2[tableau2.size() - 1][tableau2[0].size() - 1] << endl;
    cout << "Running time of Simplex Tableau 2: " << duration.count() << " microseconds" << endl;

    return 0;
}