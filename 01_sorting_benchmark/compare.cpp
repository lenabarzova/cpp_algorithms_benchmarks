#include "compare.h"

bool compareInt(const int& a, const int& b)
{
    return a < b;
}

bool compareByLength(const mystring& a, const mystring& b)
{
    return a.length() < b.length();
}

bool compareLexicographically(const mystring& a, const mystring& b)
{
    return a.get() < b.get();
}

void printIntArray(const int* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << a[i] << " ";
    }
    std::cout << "\n";
}

void printStringArray(const mystring* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << a[i].get() << "\n";
    }
}