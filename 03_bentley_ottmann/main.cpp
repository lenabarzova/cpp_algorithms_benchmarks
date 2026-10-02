#include <iostream>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

struct Point { double x, y; };
struct Segment { Point a, b; };
long long naive(const vector<Segment>& s);
long long bentley(const vector<Segment>& s);

void runTest(const char* name, const vector<Segment>& segments, long long expected){
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    long long naiveResult = naive(segments);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    long long bentleyResult = bentley(segments);
    chrono::steady_clock::time_point t3 =chrono::steady_clock::now();

    double naiveTime = chrono::duration<double, milli>(t2 - t1).count();
    double bentleyTime = chrono::duration<double, milli>(t3 - t2).count();

    std::cout << name << std::endl;
    std::cout << "Expected: " << expected << endl;
    std::cout << "Naive: " << naiveResult << " intersections, " << naiveTime << " ms" << endl;
    std::cout << "Bentley: " << bentleyResult << " intersections, " << bentleyTime << " ms" << endl;

    if (naiveResult == expected && bentleyResult == expected) {
        std::cout << "OK" << endl;
    }
    else {
        std::cout << "ERROR" << endl;
    }
    std::cout << endl;
}


int main() {
    vector<Segment> test1 = { {{0, 0}, {10, 10}},  {{0, 10}, {10, 0}} };
    runTest("Test 1: simple intersection", test1, 1);

    vector<Segment> test2 = { {{0, 0}, {10, 0}},  {{0, 2}, {10, 2}} };
    runTest("Test 2: parallel segments", test2, 0);

    vector<Segment> test3 = {
        {{0,0}, {10, 5}},
        {{0, 5}, {5,0}},
        {{0, 7}, {7,0}}
    };
    runTest("Test 3: two intersections", test3, 2);

    vector<Segment> test4 = {
        {{5, 0}, {5, 10}},
        {{0, 3}, {10, 3}},
        {{0, 7}, {10, 7}}
    };
    runTest("Test 4: vertical segment", test4, 2);

    vector<Segment> test5 = {
        {{0,0}, {5,5}},
        {{0,5}, {5,0}},
        {{5,0}, {10,5}},
        {{5,5}, {10,0}},
        {{10,0}, {15,5}},
        {{10,5}, {15,0}}
    };
    runTest("Test 5: same x intersections", test5, 7);

    const int N = 1000;
    mt19937 gen(42);
    uniform_real_distribution<double> d(0, 10000);
    vector<Segment> bigTest(N);
    for (int i = 0; i < N; i++) {
        bigTest[i].a = { 0,d(gen) };
        bigTest[i].b = { 10000, d(gen) };
    }

    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    long long naiveResult = naive(bigTest);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    long long bentleyResult = bentley(bigTest);
    chrono::steady_clock::time_point t3 = chrono::steady_clock::now();
    double naiveTime = chrono::duration<double, milli>(t2 - t1).count();
    double bentleyTime = chrono::duration<double, milli>(t3 - t2).count();

    std::cout << "Large test" << endl;
    std::cout << "Segments: " << N << endl;
    std::cout << "Naive: " << naiveResult << " intersections, " << naiveTime << " ms" << endl;
    std::cout << "Bentley: " << bentleyResult << " intersections, " << bentleyTime << " ms" << endl;

    if (naiveResult == bentleyResult) {
        std::cout << "Results are equal" << endl;
    }
    else {
        std::cout << "ERROR" << endl;
    }
    return 0;
}