#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;

const int INF = numeric_limits<int>::max();
struct Edge {
    int to, weight;
};

// Union-Find structure to manage disjoint sets
class UnionFind {
public:
    vector<int> parent, rank;
    int setCount;

    UnionFind(int n) : parent(n), rank(n, 0), setCount(n) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (u != parent[u]) {
            parent[u] = find(parent[u]);  // Path compression
        }
        return parent[u];
    }

    bool unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);

        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
            setCount--;
            return true;
        }
        return false;
    }
};

bool readGraphFromFile(const string& filename, vector<vector<Edge>>& graph, int& source, int& destination) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file!" << endl;
        return false;
    }

    string line;
    while (getline(infile, line)) {
        // Check if we've reached the '-1' marker, which indicates next two lines 
        //will be source and destination nodes
        if (line == "-1") {
            // Read source and destination nodes from the next two lines
            char source_char, destination_char;
            if (!getline(infile, line) || line.empty()) {
                cerr << "Expected source node after -1 marker but found none." << endl;
                return false;
            }
            source_char = line[0];
            if (!getline(infile, line) || line.empty()) {
                cerr << "Expected destination node after source node but found none." << endl;
                return false;
            }
            destination_char = line[0];
            source = source_char - 'a';
            destination = destination_char - 'a';
            break;
        }
        stringstream ss(line);
        //Reads vertex u as u_char, vertex v as v_char, and weight as weight
        char u_char, v_char;
        int weight;
        ss >> u_char >> v_char >> weight;
        //Converts verticies from letters to numbers. I.e a = 0, b = 1, etc
        int u = u_char - 'a';
        int v = v_char - 'a';
        // Add edges in both directions since the graph is undirected
        graph[u].push_back({v, weight});
        graph[v].push_back({u, weight});
    }
    return true;
}

void dijkstra(int source, const vector<vector<Edge>>& graph, vector<int>& dist, vector<int>& parent) {
    int n = graph.size();
    dist.assign(n, INF);
    parent.assign(n, -1);
    dist[source] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, source});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (const Edge& edge : graph[u]) {
            int v = edge.to, weight = edge.weight;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u; // Track the path
                pq.push({dist[v], v});
            }
        }
    }
}

// Helper function to reconstruct path from source to target using parent array
vector<int> reconstructPath(int target, const vector<int>& parent) {
    vector<int> path;
    for (int v = target; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}

// Function to compute shortest path from source to destination via capital node
void computeShortestPathViaCapital(int source, int capital, int destination, const vector<vector<Edge>>& graph, ofstream& outFile) {
    vector<int> distFromSource;
    vector<int> distFromCapital; 
    vector<int> parentFromSource; 
    vector<int> parentFromCapital;

    auto start = high_resolution_clock::now();
    // Run Dijkstra from source to all other nodes
    dijkstra(source, graph, distFromSource, parentFromSource); 
    // Run Dijkstra from capital to all other nodes
    dijkstra(capital, graph, distFromCapital, parentFromCapital);
    // Calculate the shortest path from source to destination via capital
    int shortestPath = distFromSource[capital] + distFromCapital[destination];
    // Reconstruct path from source to capital and from capital to destination
    vector<int> pathToCapital = reconstructPath(capital, parentFromSource);
    vector<int> pathFromCapital = reconstructPath(destination, parentFromCapital);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Print the full path from source to destination via capital
    outFile << "Shortest Path: ";
    for (int v : pathToCapital) outFile << char(v + 'a') << " ";
    for (int j = 1; j < pathFromCapital.size(); ++j) outFile << char(pathFromCapital[j] + 'a') << " ";
    outFile << endl;
    outFile << "Shortest Distance: " << shortestPath << endl;
    outFile << "Running Time: " << duration.count() << " microseconds" << endl;

}

void classifyPhotos(int numPhotos, int numCategories, vector<tuple<int, int, int>>& edges, ofstream& outFile) {
    auto start = chrono::high_resolution_clock::now();
    // Sort edges in descending order based on similarity
    sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
        return get<2>(a) > get<2>(b);
    });

    UnionFind uf(numPhotos);
    // Process edges to form clusters until we have the desired number of categories
    for (const auto& [u, v, similarity] : edges) {
        if (uf.setCount == numCategories) break;  // Stop if we have exactly numCategories clusters
        // Add edge if it connects two different components
        uf.unionSets(u, v);
    }

    // Create clusters based on the final Union-Find structure
    unordered_map<int, vector<int>> clusters;
    for (int i = 0; i < numPhotos; ++i) {
        int root = uf.find(i);
        clusters[root].push_back(i + 1);  // Photos are p1, p2, ..., so add 1 to match indexing
    }

    outFile << "Number of categories (clusters): " << clusters.size() << endl;
    int category = 1;
    for (const auto& [root, members] : clusters) {
        outFile << "Category " << category++ << " (" << members.size() << " photos): ";
        for (int member : members) {
            outFile << "p" << member << " ";
        }
        outFile << endl;
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    outFile << "Classifying Photos Runtime: " << duration.count() << " microseconds" << endl;
    outFile.close();
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];
    // string inputFile = "input.txt";
    // string outputFile = "output.txt";
     ofstream outFile(outputFile);
    vector<vector<Edge>> graph(11);
    int source;
    int destination;
    int capital = 0; //Capital is A which corresponds with 0
    readGraphFromFile(inputFile, graph, source, destination);    
    computeShortestPathViaCapital(source, capital, destination, graph, outFile);

    int numPhotos = 20;
    int numCategories = 3;
    // List of edges with (photo1, photo2, similarity)
    vector<tuple<int, int, int>> edges = {
        {0, 1, 80}, {4, 8, 73}, {8, 12, 69}, {1, 2, 10}, {4, 6, 20}, {4, 1, 74},
        {8, 17, 15}, {12, 13, 21}, {8, 0, 92}, {12, 1, 59}, {19, 3, 6}, {17, 16, 17},
        {10, 2, 74}, {6, 17, 83}, {6, 11, 27}, {0, 9, 16}, {4, 14, 47}, {19, 13, 94},
        {13, 17, 77}, {1, 3, 30}, {10, 13, 62}, {2, 19, 85}, {10, 6, 62}, {5, 9, 87},
        {11, 14, 81}, {2, 18, 39}, {8, 7, 45}, {18, 3, 65}, {7, 15, 70}, {14, 16, 64},
        {0, 10, 32}, {10, 5, 43}, {11, 5, 87}, {11, 18, 70}, {6, 15, 39}, {12, 5, 22},
        {3, 15, 68}, {16, 9, 56}, {18, 14, 71}
    };
    classifyPhotos(numPhotos, numCategories, edges, outFile);
    return 0;
}
