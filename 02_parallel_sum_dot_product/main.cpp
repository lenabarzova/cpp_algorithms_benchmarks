#include <iostream>
#include "sumscalar.h"

int main() {
    int size = 1000000;

    int* a = new int[size];
    int* b = new int[size];

    for (int i = 0; i < size; ++i) {
        a[i] = 1000;
        b[i] = 2000;
    }

    SumArray sumCalculator;
    DotProduct dotCalculator;

    long long sumResult = sumCalculator.calculateParallel(a, size);
    long long sumResult2 = sumCalculator.calculateParallel(b, size);
    long long dotResult = dotCalculator.calculateParallel(a, b, size);

    std::cout << "big array size = " << size << std::endl;
    std::cout << "each element of first array = 1000" << std::endl;
    std::cout << "each element of second array = 2000" << std::endl;
    std::cout << std::endl;

    std::cout << "parallel sum a = " << sumResult << std::endl;
    std::cout << "parallel sum b = " << sumResult2 << std::endl;
    std::cout << "parallel dot product = " << dotResult << std::endl;

    delete[] a;
    delete[] b;

    return 0;
}