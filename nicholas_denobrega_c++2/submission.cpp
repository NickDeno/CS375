#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <float.h>
#include <chrono>
#include <fstream>
using namespace std;
using namespace std::chrono;

struct Point {
    int x, y;
};

double distance(Point p1, Point p2) {
    double deltaX = p1.x - p2.x;
    double deltaY = p1.y - p2.y;

    double deltaXSquare = deltaX * deltaX;
    double deltaYSquare = deltaY * deltaY;

    double sumOfSquares = deltaXSquare + deltaYSquare;
    return sqrt(sumOfSquares);
}

double closestPairRecur(vector<Point> &points, int left, int right, pair<Point, Point> &closestPair) {
    if(right - left  == 0){
        return DBL_MAX;
    }
    
    if (right - left == 1){
        closestPair = make_pair(points[left], points[right]);
        return distance(points[left], points[right]);

    }

    int mid = left + (right - left) / 2;
    Point midPoint = points[mid];

    pair<Point, Point> leftPair, rightPair;
    double dl = closestPairRecur(points, left, mid, leftPair);
    double dr = closestPairRecur(points, mid + 1, right, rightPair);
    double dMin = (dl < dr) ? dl : dr;
    closestPair = (dl < dr) ? leftPair : rightPair;

    // Create a strip of points that are close to the dividing line(points within dMin)
    vector<Point> strip;
    for (int i = left; i <= right; ++i) {
        if (abs(points[i].x - midPoint.x) < dMin) {
            strip.push_back(points[i]);
        }
    }

    // Find the closest pair of points in the strip
    double dStrip = dMin;
    sort(strip.begin(), strip.end(), [](Point p1, Point p2) { return p1.y < p2.y; });
    for (int i = 0; i < strip.size(); ++i) {
        for (int j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < dStrip; ++j) {
            double dist = distance(strip[i], strip[j]);
            if (dist < dStrip) {
                dStrip = dist;
                closestPair = make_pair(strip[i], strip[j]);
            }
        }
    }
    return (dMin < dStrip) ? dMin : dStrip;
}

double closestPair(vector<Point> &points, pair<Point, Point> &closestPair) {
    vector<Point> pointsCopy = points;
    sort(pointsCopy.begin(), pointsCopy.end(), [](Point p1, Point p2) { return p1.x < p2.x; });
    return closestPairRecur(pointsCopy, 0, pointsCopy.size()-1, closestPair);
}

double closestPairBrute(vector<Point> &points, pair<Point, Point> &closestPair) {
    double minDistance = DBL_MAX;
    int n = points.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dist = distance(points[i], points[j]);
            if (dist < minDistance) {
                minDistance = dist;
                closestPair = make_pair(points[i], points[j]);
            }
        }
    }
    return minDistance;
}

int findIndex(const vector<Point>& points, const Point& target) {
    for (int i = 0; i < points.size(); ++i) {
        if (points[i].x == target.x && points[i].y == target.y) {
            return i;
        }
    }
    return -1;
}

// Example test cases:
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    pair<Point, Point> cPair;
    double distance;

    //Input File Case
    string inputFile = argv[1];
    string outputFile = argv[2];
    ifstream infile(inputFile);
    vector<Point> points;
    int x, y;
    while (infile >> x >> y) {
        points.push_back({x, y});
    }
    infile.close();
    distance = closestPair(points, cPair);
    int index1 = findIndex(points, cPair.first);
    int index2 = findIndex(points, cPair.second);
    ofstream outfile(outputFile);
    outfile << index1 << " " << index2 << " " << distance << endl;
    outfile.close();

    //Test case 1
    int n = 10000;
    vector<Point> testPoints1;
    for (int i = 0; i < n; ++i) {
        Point p;
        testPoints1.push_back({n - i, n - i});
    }

    // Test case 2
    vector<Point> testPoints2;
    for (int i = 0; i < n; ++i) {
        testPoints2.push_back({i * i, i * i});
    }

    //Test Case 1: Divide and Conquer Approach
    auto start = high_resolution_clock::now();
    distance = closestPair(testPoints1, cPair);
    auto end = high_resolution_clock::now();
    auto divideDuration = duration_cast<microseconds>(end - start);
    cout << "Test Case 1 Divide and Conquer - Minimum distance: " << distance << ", Closest pair: (" 
         << cPair.first.x << ", " << cPair.first.y << ") and (" 
         << cPair.second.x << ", " << cPair.second.y << ")\n"; 
    //Test Case 2: Divide and Conquer Approach
    start = high_resolution_clock::now();
    distance = closestPair(testPoints2, cPair);
    end = high_resolution_clock::now();
    divideDuration = duration_cast<microseconds>(end - start);
    cout << "Test Case 2 Divide and Conquer - Minimum distance: " << distance << ", Closest pair: (" 
         << cPair.first.x << ", " << cPair.first.y << ") and (" 
         << cPair.second.x << ", " << cPair.second.y << ")\n";

    //Test Case 1: Brute Force Approach
    start = high_resolution_clock::now();
    distance = closestPairBrute(testPoints1, cPair);
    end = high_resolution_clock::now();
    auto bruteDuration = duration_cast<microseconds>(end - start);
    cout << "Test Case 1 Brute Force - Minimum distance: " << distance << ", Closest pair: (" 
         << cPair.first.x << ", " << cPair.first.y << ") and (" 
         << cPair.second.x << ", " << cPair.second.y << ")\n";  
    //Test Case 2: Brute Force Approach
    start = high_resolution_clock::now();
    distance = closestPairBrute(testPoints2, cPair);
    end = high_resolution_clock::now();
    bruteDuration = duration_cast<microseconds>(end - start);
    cout << "Test Case 2 Brute Force - Minimum distance: " << distance << ", Closest pair: (" 
         << cPair.first.x << ", " << cPair.first.y << ") and (" 
         << cPair.second.x << ", " << cPair.second.y << ")\n";

    //Compare the time cost of each algorithm
    cout << "\nTest Case 1 Brute Force Time: " << bruteDuration.count() << " microseconds\n";
    cout << "Test Case 1 Divide and Conquer Time: " << divideDuration.count() << " microseconds\n";
    cout << "Test Case 2 Brute Force Time: " << bruteDuration.count() << " microseconds\n";
    cout << "Test Case 2 Divide and Conquer Time: " << divideDuration.count() << " microseconds\n";

    return 0;
}