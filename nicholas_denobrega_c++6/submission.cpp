#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

unordered_map<char, unordered_map<char, int>> graph = {
    {'A', {{'B', 4}, {'C', 3}, {'D', 2}}},
    {'B', {{'A', 4}, {'C', 1}, {'D', 2}}},
    {'C', {{'A', 3}, {'B', 1}, {'D', 5}}},
    {'D', {{'A', 2}, {'B', 2}, {'C', 5}}}
};

// Helper function for MST heuristic
int mst_heuristic(const vector<char>& tour, const unordered_set<char>& unvisited) {
    int cost = 0;
    unordered_set<char> visited(tour.begin(), tour.end());
    unordered_set<char> remaining = unvisited;
    while (!remaining.empty()) {
        int min_edge = INT_MAX;
        char selected_u, selected_v;
        // Find the minimum edge connecting visited and unvisited nodes
        for (char u : visited) {
            for (auto& [v, weight] : graph[u]) {
                if (remaining.find(v) != remaining.end() && weight < min_edge) {
                    min_edge = weight;
                    selected_u = u;
                    selected_v = v;
                }
            }
        }
        // No more edges to consider
        if (min_edge == INT_MAX) break;
        cost += min_edge;
        visited.insert(selected_v);
        remaining.erase(selected_v);
    }
    return cost;
}

// Best-first search algorithm
vector<char> best_first_search(char start) {
    vector<char> tour = {start};
    unordered_set<char> unvisited;
    for (auto& node : graph) {
        if (node.first != start) {
            unvisited.insert(node.first);
        }
    }
    // Priority queue for best-first search
    priority_queue<pair<int, vector<char>>, vector<pair<int, vector<char>>>, greater<>> heap;
    heap.push({mst_heuristic(tour, unvisited), tour});
    while (!heap.empty()) {
        auto [_, current_tour] = heap.top();
        heap.pop();

        // Check if we have visited all cities
        if (unvisited.empty()) {
            current_tour.push_back(start); // Return to the start city
            return current_tour;
        }

        // Generate successors
        for (char city : unvisited) {
            vector<char> new_tour = current_tour;
            new_tour.push_back(city);
            unordered_set<char> new_unvisited = unvisited;
            new_unvisited.erase(city);
            int new_cost = mst_heuristic(new_tour, new_unvisited);
            heap.push({new_cost, new_tour});
        }

        // Mark the last city in the current tour as visited
        if (!current_tour.empty()) {
            unvisited.erase(current_tour.back());
        }
    }

    return {}; // No complete tour found
}

// Main function
int main() {
    char start = 'A';
    vector<char> optimal_tour = best_first_search(start);

    // Output the optimal tour
    if (!optimal_tour.empty()) {
        cout << "Optimal Tour: ";
        for (char city : optimal_tour) {
            cout << city << " ";
        }
        cout << endl;
    } else {
        cout << "No tour found." << endl;
    }

    return 0;
}
