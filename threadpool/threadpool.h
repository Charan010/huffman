#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:

    explicit ThreadPool(size_t n);

    ~ThreadPool();

    void submit(std::function<void()> job);

    void shutdown();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;

    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;
};

#endif 