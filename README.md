# cpp_algorithms_benchmarks
A collection of C++ programming assignments focused on algorithm implementation, multithreading, performance benchmarking, and working with large datasets.

The repository contains several independent projects. Each project implements an algorithm or computational task from scratch and, where applicable, compares its performance with standard or alternative approaches.

## Projects
### 1. Generic Sorting & STL Benchmark
A C++ project that implements a generic Merge Sort recursive algorithm using templates and custom comparator functions, and compares its performance with the C++ Standard Library std::sort. The custom sorting algorithm is implemented as a reusable template that accepts a comparator function, allowing the same Merge Sort implementation to work with different data types and sorting rules. Execution time is measured using std::chrono::steady_clock. The program benchmarks both algorithms for string sorting by length, lexicographical string sorting, and integer sorting. 

### 2. Parallel Array Sum & Vector Dot Product
A C++ project that implements parallel computation of the sum of array elements and the dot product of two vectors using std::thread. The functionality is encapsulated in SumArray and DotProduct classes with a configurable number of worker threads. The input data is divided into approximately equal blocks, and each thread calculates a local partial result. Shared results are synchronized using std::mutex to prevent race conditions. The project demonstrates multithreading, workload distribution, thread synchronization, and parallel reduction operations in C++.

### 3. Bentley–Ottmann & Parallel Naive Intersection Search
A C++ project that implements the Bentley–Ottmann sweep-line algorithm for detecting intersections between line segments and compares it with a parallelized naive approach. The Bentley–Ottmann implementation uses an event queue, an ordered active set of segments, neighbor checks, and scheduled intersection events to reduce unnecessary comparisons. The naive algorithm checks segment pairs for intersections but distributes the work across a custom thread pool using std::thread, std::mutex, and std::condition_variable. 
The implementation is validated on several test cases, including a simple intersection, parallel segments, multiple intersections, vertical segments, and multiple intersections sharing the same x-coordinate. A larger randomized test with 1000 segments is also used to verify that both algorithms produce identical results and to compare their execution time.

### 4. External Merge Sort for Large Datasets
A C++ project that implements external sorting for datasets that are too large to fit entirely into memory. The input file is split into fixed-size chunks, each chunk is sorted in memory using a custom recursive Merge Sort implementation, and the sorted chunks are written to temporary files. Intermediate data is stored in temporary disk files, so the program does not need to load the entire dataset into memory at once, allowing it to process files much larger than the available RAM.
The temporary sorted files are then merged pairwise until a single fully sorted output file remains. The implementation uses a custom thread pool based on std::thread, std::mutex, and std::condition_variable to parallelize both chunk sorting and file merging. Execution time is measured with std::chrono::steady_clock, and the program benchmarks several thread counts with repeated runs to compare performance.
