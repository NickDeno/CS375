#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

//Function to read data from input file
vector<vector<int>> readData(const string& filename) {
    vector<vector<int>> data;
    vector<int> data1;
    vector<int> data2;

    ifstream infile(filename);
    int val;
    while (infile >> val) {
        if(val == -1){
            break;
        }
        data1.push_back(val);
    }
    while(infile >> val){
        data2.push_back(val);
    }
    data.push_back(data1);
    data.push_back(data2);
    return data;
}

//B1
void findDifferenceKeys(const vector<int>& arr, const string& outputFile) {
    unordered_set<int> arrSet(arr.begin(), arr.end()); 
    ofstream outFile(outputFile);
    
    auto start = high_resolution_clock::now();
    bool found = false;
    for (int i = 0; i < arr.size(); i++) {
        for (int j = i + 1; j < arr.size(); j++) {
            int diff = abs(arr[i] - arr[j]);
            if (arrSet.find(diff) != arrSet.end()) { 
                int larger = max(arr[i], arr[j]);
                int smaller = min(arr[i], arr[j]);
                outFile << diff << " " << larger << " " << smaller << endl;
                found = true;
            }
        }
    }

    if (!found) {
        outFile.close();
        return;
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    outFile << "Running time: " << duration.count() / 1e6 << " seconds" << endl;
    outFile.close();
}

//B2
void findAllPairsWithSum(const vector<int>& arr, int targetSum, const string& outputFile) {
    ofstream outFile(outputFile);
    vector<int> sortedArr = arr;
    sort(sortedArr.begin(), sortedArr.end());
    
    int left = 0;
    int right = sortedArr.size() - 1;
    bool found = false;
    auto start = high_resolution_clock::now();
    while (left < right) {
        int currentSum = sortedArr[left] + sortedArr[right];
        if (currentSum == targetSum) {
            int larger = max(sortedArr[left], sortedArr[right]);
            int smaller = min(sortedArr[left], sortedArr[right]);
            outFile << targetSum << " " << larger << " " << smaller << endl;
            left++;
            right--;
            found = true;
        }
        else if (currentSum < targetSum) {
            left++;
        }
        else {
            right--;
        }
    }

    if (!found) {
        outFile.close();
        return;
    }

    auto end = high_resolution_clock::now(); // End timing
    auto duration = duration_cast<microseconds>(end - start);
    outFile << "Running time: " << duration.count() / 1e6 << " seconds" << endl;
}

int main(int argc, char* argv[]){
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " inputFile.txt outputFile.txt outputFile2.txt" << endl;
        return 1;
    }

    string inputFile = argv[1];
    string outputFile = argv[2];
    string outputFile2 = argv[3];
    vector<vector<int>> data = readData(inputFile);
    findDifferenceKeys(data[0], outputFile);
    int targetSum = data[1].at(data[1].size()-1);
    data[1].pop_back();
    findAllPairsWithSum(data[1], targetSum, outputFile2);
    return 0;
}