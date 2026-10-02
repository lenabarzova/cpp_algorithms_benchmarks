#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
using namespace std;

struct Point { double x, y; };
struct Segment { Point a, b; };




class ThreadPool {
private:
    vector<thread> workers;
    queue<function<void()>> tasks;

    mutex mtx;
    condition_variable cv;
    condition_variable finishedCv;

    bool stop;
    int activeTasks;

    void workerLoop();

public:
    ThreadPool(int threadCount);
    ~ThreadPool();

    void addTask(function<void()> task);
    void waitAll();
};


ThreadPool::ThreadPool(int threadCount) {
    stop = false;
    activeTasks = 0;

    for (int i = 0; i < threadCount; i++) {
        workers.push_back(thread(&ThreadPool::workerLoop, this));
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
                hastask = true;
            }

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


bool onSegment(Point a, Point b, Point p) {
    return (p.x - a.x) * (p.x - b.x) <= 0 && (p.y - a.y) * (p.y - b.y) <= 0;
}

double cross(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool intersect(const Segment& a, const Segment& b) {
    if (cross(a.a, a.b, b.a) * cross(a.a, a.b, b.b) < 0 &&
        cross(b.a, b.b, a.a) * cross(b.a, b.b, a.b) < 0) {
        return true;
    }
    return 
        (cross(a.a, a.b, b.a) == 0 && onSegment(a.a, a.b, b.a)) ||
        (cross(a.a, a.b, b.b) == 0 && onSegment(a.a, a.b, b.b)) ||
        (cross(b.a, b.b, a.a) == 0 && onSegment(b.a, b.b, a.a)) ||
        (cross(b.a, b.b, a.b) == 0 && onSegment(b.a, b.b, a.b));
}

long long naive(const vector<Segment>& s) {
    int threadCount = thread::hardware_concurrency();
    if (threadCount == 0) {
        threadCount = 4;
    }
    ThreadPool pool(threadCount);
    vector<long long> results(threadCount, 0);

    for (int id = 0; id < threadCount; id++) {
        pool.addTask([&, id]() {
            long long count = 0;
            for (int i = id; i < (int)s.size(); i += threadCount) {
                for (int j = i + 1; j < (int)s.size(); j++) {
                    if (intersect(s[i], s[j])) {
                        count++;
                    }
                }
            }
            results[id] = count;
            });
    }
    pool.waitAll();
    long long ans = 0;

    for (int i = 0; i < results.size(); i++) {
        ans += results[i];
    }
    return ans;
}