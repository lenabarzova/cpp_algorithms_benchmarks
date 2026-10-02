#include "sumscalar.h"

void sumBlock(const int* data, int left, int right, long long* result, std::mutex* m){

    long long localSum = 0;

    for (int i = left; i < right; ++i) {
        localSum += data[i];
    }

    m->lock();
    *result += localSum;
    m->unlock();
}

void dotBlock(const int* first, const int* second, int left, int right, long long* result, std::mutex* m){

    long long localDot = 0;

    for (int i = left; i < right; ++i) {
        localDot += (long long)first[i] * second[i];
    }

    m->lock();
    *result += localDot;
    m->unlock();
}





void SumArray::addSum(long long value) {
    msum.lock();
    sumresult += value;
    msum.unlock();
}

long long SumArray::getSumResult() const {
    return sumresult;
}

long long SumArray::calculateParallel(const int* data, int size, int threadCount) {

    sumresult = 0;

    std::thread* allthreads = new std::thread[threadCount];

    int blockSize = size / threadCount;
    int extra = size % threadCount;
    int left = 0;

    for (int i = 0; i < threadCount; ++i) {
        int currentBlockSize = blockSize;

        if (i < extra) {
            currentBlockSize++;
        }

        int right = left + currentBlockSize;

        allthreads[i] = std::thread(sumBlock, data, left, right, &sumresult, &msum);

        left = right;
    }

    for (int i = 0; i < threadCount; ++i) {
        allthreads[i].join();
    }

    long long result = sumresult;

    delete[] allthreads;

    return result;
}

void DotProduct::addDot(long long value) {
    mdot.lock();
    dotresult += value;
    mdot.unlock();
}

long long DotProduct::getDotResult() const {
    return dotresult;
}

long long DotProduct::calculateParallel(const int* first, const int* second, int size, int threadCount) {
    dotresult = 0;

    std::thread* allthreads = new std::thread[threadCount];

    int blockSize = size / threadCount;
    int extra = size % threadCount;
    int left = 0;

    for (int i = 0; i < threadCount; ++i) {
        int currentBlockSize = blockSize;

        if (i < extra) {
            currentBlockSize++;
        }

        int right = left + currentBlockSize;

        allthreads[i] = std::thread(dotBlock, first, second, left, right, &dotresult, &mdot);

        left = right;
    }

    for (int i = 0; i < threadCount; ++i) {
        allthreads[i].join();
    }

    long long result = dotresult;

    delete[] allthreads;

    return result;
}