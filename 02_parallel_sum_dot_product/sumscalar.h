#pragma once
# include <thread>
#include <mutex>

class SumArray {
private:
	long long sumresult = 0;
	std::mutex msum;
public:
	void addSum(long long value);
	long long getSumResult() const;
	long long calculateParallel(const int* data, int size, int threadCount = 4);
};

class DotProduct {
private:
	long long dotresult = 0;
	std::mutex mdot;
public:
	void addDot(long long value);
	long long getDotResult() const;
	long long calculateParallel(const int* first, const int* second, int size, int threadCount = 4);
};

void sumBlock(const int* data, int left, int right, long long* result, std::mutex* m);

void dotBlock(const int* first, const int*  second, int left, int right, long long* result, std::mutex* m);