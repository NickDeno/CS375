#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
using namespace std;
using namespace std::chrono; 

const int INF = numeric_limits<int>::max();

void floyd(int n, vector<vector<int>>& D, vector<vector<int>>& P) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (D[i][j] > D[i][k] + D[k][j] && D[i][k] != INF && D[k][j] != INF) {
                    D[i][j] = D[i][k] + D[k][j];
                    P[i][j] = k;
                }
            }
        }
    }
}

// Function to find the LCS and build the matrices c (length) and b (arrows)
void LCS(const string& x, const string& y, vector<vector<int>>& c, vector<vector<char>>& b) {
    int m = x.size();
    int n = y.size();

    // Initialize the first row and column of the c and b matrices to 0
    for (int i = 0; i <= m; ++i) c[i][0] = 0;
    for (int j = 0; j <= n; ++j) c[0][j] = 0;

    // Fill in the LCS matrix
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (x[i-1] == y[j-1]) {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 'D'; // Diagonal
            } else if (c[i-1][j] >= c[i][j-1]) {
                c[i][j] = c[i-1][j];
                b[i][j] = 'U'; // Up
            } else {
                c[i][j] = c[i][j-1];
                b[i][j] = 'L'; // Left
            }
        }
    }
}

void printPath(int q, int r, const vector<vector<int>>& P) {
    if (P[q][r] != 0) {
        printPath(q, P[q][r], P); // Recursively go to the previous node in path
        cout << "v" << P[q][r] << " "; // Print intermediate node
        printPath(P[q][r], r, P);
    }
}

void printFloydMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& val : row) {
            if (val == INF) cout << "INF ";
            else cout << val << " ";
        }
        cout << endl;
    }
}


// Function to print the LCS by tracing back through the b matrix
void printLCS(const vector<vector<char>>& b, const string& x, int i, int j) {
    if (i == 0 || j == 0){
        return;
    }

    if (b[i][j] == 'D') {
        printLCS(b, x, i - 1, j - 1);
        cout << x[i - 1];
    } else if (b[i][j] == 'U') {
        printLCS(b, x, i - 1, j);
    } else {
        printLCS(b, x, i, j - 1);
    }
}

void printLCSMatrices(const vector<vector<int>>& c, const vector<vector<char>>& b, int m, int n) {
    cout << "\nLCS Length Matrix (c):\n";
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            cout << c[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nArrow Matrix (b):\n";
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            cout << b[i][j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    vector<vector<int>> D = {
        {0,140, INF, INF, INF, 180,100,70, INF, INF, INF},
        {140,0,130, INF, 100, INF,INF,INF,INF,INF,INF},
        {INF, 130, 0, 160, INF,INF,INF,INF,INF,INF,INF},
        {INF,INF, 160, 0, 160,INF,INF,INF,INF,INF, 180},
        {INF, 100, INF, 65, 0, 70, INF, INF, 70, INF,INF},
        {180, INF,INF,INF,70, 0,60,INF,INF,INF,INF},
        {100,INF,INF,INF,INF,60,0,65,INF,INF,INF},
        {70,INF,INF,INF,INF,INF,65,0,INF,INF,INF},
        {INF,INF,INF,INF,70,INF,INF,INF,0,60,INF},
        {INF,INF,INF,INF,INF,INF,INF,INF,60,0,100},
        {INF,INF,INF,180,INF,INF,INF,INF,INF,100,0}
    };
    vector<vector<int>> P(11, vector<int>(11, 0));
    auto start = high_resolution_clock::now();
    floyd(11, D, P);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    cout << "\nRunning time: " << duration << " ms" << endl;
    cout << "Final D table (shortest distances):" << endl;
    printFloydMatrix(D);
    cout << "Final P table (intermediate nodes):" << endl;
    printFloydMatrix(P);
    // Print all paths between any two cities
    cout << "All shortest paths between any two cities:" << endl;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (i != j && D[i][j] < INF) {
                cout << "Shortest path from v" << i+1 << " to v" << j+1 << ": v" << i+1 << " ";
                printPath(i, j, P);
                cout << "v" << j+1 << " (Distance: " << D[i][j] << ")" << endl;
            }
        }
    }

    //Extra Credit LCS 6%:
    string x = "CACMYCCA";
    string y = "YMCMAMYYCMA";
    int m = x.size();
    int n = y.size();
    // Create LCS length (c) and arrow (b) matrices
    vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));
    vector<vector<char>> b(m + 1, vector<char>(n + 1, 0));
    // Compute the LCS
    LCS(x, y, c, b);
    // Print the LCS length and arrow matrices
    printLCSMatrices(c, b, m, n);
    //Print LCS
    cout << "\nThe Longest Common Subsequence is: ";
    printLCS(b, x, m, n);
    cout << endl;


    return 0;


}