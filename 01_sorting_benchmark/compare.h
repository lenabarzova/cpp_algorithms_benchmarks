#pragma once
#include <iostream>
#include "mystring.h"

bool compareInt(const int& a, const int& b);
bool compareByLength(const mystring& a, const mystring& b);
bool compareLexicographically(const mystring& a, const mystring& b);

template <typename T>
void merge(T* a, int left, int mid, int right, bool (*cmp)(const T&, const T&)) 
//cmp is a pointer to a function, which receives 2 objs type T from const link and returns type bool 
{
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
void mergeSortRecursive(T* a, int left, int right, bool (*cmp)(const T&, const T&))
{
    if (left >= right)
    {
        return;
    }

    int mid = (left + right) / 2;

    mergeSortRecursive(a, left, mid, cmp);
    mergeSortRecursive(a, mid + 1, right, cmp);
    merge(a, left, mid, right, cmp);
}

template <typename T>
void mergeSort(T* a, int n, bool (*cmp)(const T&, const T&))
{
    if (n > 1)
    {
        mergeSortRecursive(a, 0, n - 1, cmp);
    }
}

void printIntArray(const int* a, int n);
void printStringArray(const mystring* a, int n);