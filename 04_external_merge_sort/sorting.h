#pragma once

#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>




bool compareLongLong(const long long& a, const long long& b);




template <typename T>
void merge(T* a, int left, int mid, int right, bool (*cmp)(const T&, const T&)) {
//cmp is a pointer to a function, which receives 2 objs type T from const link and returns type bool 
    int n1 = mid - left + 1;
    int n2 = right - mid;
    T* l = new T[n1];
    T* r = new T[n2];
    for (int i = 0; i < n1; i++)
    {
        l[i] = a[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        r[j] = a[mid + 1 + j];
    }
    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2)
    {
        if (cmp(l[i], r[j]))
        {
            a[k] = l[i];
            i++;
        }
        else
        {
            a[k] = r[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        a[k] = l[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        a[k] = r[j];
        j++;
        k++;
    }
    delete[] l;
    delete[] r;
}

template <typename T>
void mergeSortRecursive(T* a, int left, int right, bool (*cmp)(const T&, const T&)){
    if (left >= right){ return;}
        int mid = (left + right) / 2;
        mergeSortRecursive(a, left, mid, cmp);
        mergeSortRecursive(a, mid + 1, right, cmp);
        merge(a, left, mid, right, cmp);
}

template <typename T>
void mergeSort(T* a, int n, bool (*cmp)(const T&, const T&)){
    if (n > 1)
    {
        mergeSortRecursive(a, 0, n-1, cmp);
    }
}









class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()> > tasks;

    std::mutex mtx;
    std::condition_variable cv;
    std::condition_variable finishedCv;

    bool stop;
    int activeTasks;

    void workerLoop();

public:
    ThreadPool(int threadCount);
    ~ThreadPool();

    void addTask(std::function<void()> task);
    void waitAll();
};








class ExternalSorter {

private:
    std::string inputFile;
    std::string outputFile;

    int chunkSize;
    int threadCount;
    int tempFileCounter;

    std::string createTempFileName();

    void sortChunk(std::vector<long long> chunk, const std::string& fileName);
  
    void mergeFiles(const std::string& file1, const std::string& file2, const std::string& resultFile);

    std::vector<std::string> splitFile(ThreadPool& pool);
    std::string mergeAllFiles(std::vector<std::string> files, ThreadPool& pool);



public:
    ExternalSorter(const std::string& inFile, const std::string& outFile, int chunk, int threads);
    void sort();

};