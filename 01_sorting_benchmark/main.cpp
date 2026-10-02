#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>
#include "mystring.h"
#include "compare.h"

int countLines(const std::string& fileName)
{
    std::ifstream fin(fileName);
    std::string line;
    int count = 0;

    while (std::getline(fin, line))
    {
        count++;
    }

    return count; // counting how many lines are in txt file 
}

void readStringsFromFile(const std::string& fileName, mystring* arr, int n)
{
    std::ifstream fin(fileName);
    std::string line;
    int i = 0;

    while (i < n && std::getline(fin, line))
    {
        arr[i] = mystring(line); //every line from txt is now an object of "mystring" class and lives in array "arr"; mystraing(value) is a prvalue
        i++;
    }
}

void copyStringArray(const mystring* from, mystring* to, int n)
{
    for (int i = 0; i < n; i++)
    {
        to[i] = from[i]; //copying every element because we don't wanna ruin our data
    }
}

void copyIntArray(const int* from, int* to, int n)
{
    for (int i = 0; i < n; i++)
    {
        to[i] = from[i];
    }
}
int main()
{
    std::string fileName = "text.txt"; // lvalue
    int n = countLines(fileName);  // lvalue

    if (n == 0)
    {
        std::cout << "file is empty/file is not opening\n";
        return 1;
    }

    mystring* a1 = new mystring[n]; //merge sort as length
    mystring* a2 = new mystring[n]; //std::sort as length
    mystring* a3 = new mystring[n]; 
    mystring* a4 = new mystring[n];

    readStringsFromFile(fileName, a1, n);
    copyStringArray(a1, a2, n);
    copyStringArray(a1, a3, n);
    copyStringArray(a1, a4, n);

    int* b1 = new int[n];
    int* b2 = new int[n];

    for (int i = 0; i < n; i++)
    {
        b1[i] = n - i; //just reverse order
    }

    copyIntArray(b1, b2, n);

   // std::cout << "strings from file without sort:\n";
   // printStringArray(a1, n);
   // std::cout << "\n";

    std::chrono::steady_clock::time_point start1 = std::chrono::steady_clock::now();
    mergeSort(a1, n, compareByLength);
    std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();

    std::chrono::steady_clock::time_point start2 = std::chrono::steady_clock::now();
    std::sort(a2, a2 + n, compareByLength);
    std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();

    std::chrono::steady_clock::time_point start3 = std::chrono::steady_clock::now();
    mergeSort(a3, n, compareLexicographically);
    std::chrono::steady_clock::time_point end3 = std::chrono::steady_clock::now();

    std::chrono::steady_clock::time_point start4 = std::chrono::steady_clock::now();
    std::sort(a4, a4 + n, compareLexicographically);
    std::chrono::steady_clock::time_point end4 = std::chrono::steady_clock::now();

    std::chrono::steady_clock::time_point start5 = std::chrono::steady_clock::now();
    mergeSort(b1, n, compareInt);
    std::chrono::steady_clock::time_point end5 = std::chrono::steady_clock::now();

    std::chrono::steady_clock::time_point start6 = std::chrono::steady_clock::now();
    std::sort(b2, b2 + n, compareInt);
    std::chrono::steady_clock::time_point end6 = std::chrono::steady_clock::now();


    std::chrono::duration<double, std::milli> t1 = end1 - start1; // end1, start1, t1 - lvalue; (end1 - start1) - prvalue
    std::chrono::duration<double, std::milli> t2 = end2 - start2;
    std::chrono::duration<double, std::milli> t3 = end3 - start3;
    std::chrono::duration<double, std::milli> t4 = end4 - start4;
    std::chrono::duration<double, std::milli> t5 = end5 - start5;
    std::chrono::duration<double, std::milli> t6 = end6 - start6;


    std::cout << "merge sort as length:\n";
    printStringArray(a1, n);
    std::cout << "\n";
    /*
    std::cout << "std::sort as length:\n";
    printStringArray(a2, n);
    std::cout << "\n";

    std::cout << "merge sort as lexicographically:\n";
    printStringArray(a3, n);
    std::cout << "\n";

    std::cout << "std::sort as lexicographically:\n";
    printStringArray(a4, n);
    std::cout << "\n"; */

    std::cout << "Time merge sort as length: " << t1.count() << " ms\n";
    std::cout << "Time std::sort as length: " << t2.count() << " ms\n";
    std::cout << "Time merge sort as lexicographically: " << t3.count() << " ms\n";
    std::cout << "Time std::sort as lexicographically: " << t4.count() << " ms\n";
    std::cout << "Time merge sort int: " << t5.count() << " ms\n";
    std::cout << "Time std::sort int: " << t6.count() << " ms\n";


    delete[] a1;
    delete[] a2;
    delete[] a3;
    delete[] a4;
    delete[] b1;
    delete[] b2;

    return 0;
}