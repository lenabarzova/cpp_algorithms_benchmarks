#include <iostream>
#include "sorting.h"
#include <chrono>
#include <thread>
using namespace std;

int main() {
    int maxThreads = thread::hardware_concurrency();
    if (maxThreads == 0) {maxThreads = 4;}
    int chunkSize = 50000;
    int repeats = 3;
    int threadCounts[] = { 12, 13, 14, 15 };
    cout << "Max hardware threads: " << maxThreads << endl;
    cout << "Chunk size: " << chunkSize << endl;
    cout << endl;

    for (int i = 0; i < 4; i++) {
        int threads = threadCounts[i];
        double sumTime = 0;
        cout << "Threads: " << threads << endl;

        for (int j = 0; j < repeats; j++) {
            ExternalSorter sorter("input.txt", "res11.txt", chunkSize, threads);
            chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
            sorter.sort();
            chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
            double time = chrono::duration<double, milli>(t2 - t1).count();
            sumTime += time;
            cout << "  Run " << j + 1 << ": " << time << " ms" << endl;
        }
        cout << "  Average: " << sumTime / repeats << " ms" << endl;
        cout << endl;
    }
    cout << "Testing finished." << endl;
    return 0;
}