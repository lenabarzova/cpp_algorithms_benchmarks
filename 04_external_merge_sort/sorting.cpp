#include "sorting.h"

#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

ThreadPool::ThreadPool(int threadCount) {
    stop = false;
    activeTasks = 0;
    for (int i = 0; i < threadCount; i++) {
        workers.push_back(std::thread(&ThreadPool::workerLoop, this));
    }
}

void ThreadPool::workerLoop() {
    bool flag = true;
    while (flag) {
        std::function<void()> task;
        bool hastask = false;
        {
            std::unique_lock<std::mutex> lock(mtx);
            while (!stop && tasks.empty()) {
                cv.wait(lock);
            }
            if (stop && tasks.empty()) {
                flag = false;
            }
            else {
            task = tasks.front();
            tasks.pop();
            hastask = true;}

        }
        if (hastask) {
            task();
            {
                std::unique_lock<std::mutex> lock(mtx);
                activeTasks--;
                if (activeTasks == 0) {
                    finishedCv.notify_all();
                }
            }
        }
    }
}
ThreadPool::~ThreadPool() {
    {
        unique_lock<mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for (int i = 0; i < (int)workers.size(); i++) {
        if (workers[i].joinable()) {
            workers[i].join();
        }
    }
}

void ThreadPool::addTask(function<void()> task) {
    {
        unique_lock<mutex> lock(mtx);
        tasks.push(task);
        activeTasks++;
    }
    cv.notify_one();
}

void ThreadPool::waitAll() {
    unique_lock<mutex> lock(mtx);
    while (activeTasks != 0) {
        finishedCv.wait(lock);
    }
}

bool compareLongLong(const long long& a, const long long& b) {
    return a <= b;
}










ExternalSorter::ExternalSorter(const string& inFile, const string& outFile, int chunk, int threads) {
    inputFile = inFile;
    outputFile = outFile;
    chunkSize = chunk;
    threadCount = threads;
    tempFileCounter = 0;
}

string ExternalSorter::createTempFileName() {
    string name = "temp_" + to_string(tempFileCounter) + ".txt";
    tempFileCounter++;
    return name;
}

void ExternalSorter::sortChunk(vector<long long> chunk,const string& fileName) {
    mergeSort(chunk.data(), (int)chunk.size(), compareLongLong);
    ofstream out(fileName);
    for (int i = 0; i < (int)chunk.size(); i++) {
        out << chunk[i]<< '\n';
    }
}

void ExternalSorter::mergeFiles(const string& file1, const string& file2, const string& resultFile) {
    ifstream in1(file1);
    ifstream in2(file2);
    ofstream out(resultFile);
    long long a;
    long long b;
    bool hasA = bool(in1 >> a);
    bool hasB = bool(in2 >> b);

    while (hasA && hasB) {
        if (a <= b) {
            out << a << '\n';
            hasA = bool(in1 >> a);
        }
        else {
            out << b << '\n';
            hasB = bool(in2 >> b);
        }
    }

    while (hasA) {
        out << a << '\n';
        hasA = bool(in1 >> a);
    }
    while (hasB) {
        out << b << '\n';
        hasB = bool(in2 >> b);
    }
}

vector<string> ExternalSorter::splitFile(ThreadPool& pool) {
    ifstream input(inputFile); 
    vector<string> files;

    if (!input.is_open()) {
        cout << "Could not open input file" << endl;
        return files;
    } 
    bool inputFinished = false;
    while (!inputFinished) {
        int tasksInBatch = 0;
        for (int i = 0; i < threadCount && !inputFinished; i++) {
            vector<long long> chunk;
            long long x;
            while ((int)chunk.size() < chunkSize && input >> x) {
                chunk.push_back(x);
            }
            if (chunk.empty()) {
                inputFinished = true;
            }
            else {
                string fileName = createTempFileName();
                files.push_back(fileName);
                pool.addTask([this, chunk, fileName]() {sortChunk(chunk, fileName); });
                tasksInBatch++;
            }
        }
        if (tasksInBatch > 0) {
            pool.waitAll();
        }
    }
    input.close();
    return files;
}

string ExternalSorter::mergeAllFiles(vector<string> files, ThreadPool& pool) {
    while (files.size() > 1) {
        vector<string> newFiles;
        vector<string> filesToDelete;
        for (int i = 0; i < (int)files.size(); i += 2) {
            if (i + 1 == (int)files.size()) {
                newFiles.push_back(files[i]);
            }
            else {
                string file1 = files[i];
                string file2 = files[i + 1];
                string resultFile = createTempFileName();
                newFiles.push_back(resultFile);
                filesToDelete.push_back(file1);
                filesToDelete.push_back(file2);
                pool.addTask([this, file1, file2, resultFile]() { mergeFiles(file1, file2, resultFile); });
            }
        }

        pool.waitAll();
        for (int i = 0; i < (int)filesToDelete.size(); i++) {
            remove(filesToDelete[i].c_str());
        }
        files = newFiles;
    }
    return files[0];
}

void ExternalSorter::sort() {
    remove(outputFile.c_str());
    ThreadPool pool(threadCount);
    vector<string> files = splitFile(pool);
    if (files.empty()) {
        ofstream out(outputFile);
        out.close();
        return;
    }
    string resultFile = mergeAllFiles(files, pool);
    if (rename(resultFile.c_str(), outputFile.c_str()) != 0) {
        cout << "Could not rename result file" << endl;
    }
}
