#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t num_threads) : stop(false) {
        for(size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this]() {
                while(true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(mtx);

                        cv.wait(lock, [this]() {
                            return stop || !tasks.empty();
                        });

                        if(stop && tasks.empty())
                            return;

                        // using move semantics to simply steal the func pointer.
                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            tasks.push(std::move(task));
        }
        
        cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop = true;
        }

        cv.notify_all();

        for(auto &t : workers)
            t.join();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
};


int main(){

    unsigned int threadpool_size = std::thread::hardware_concurrency();

    if(threadpool_size == 0)
        threadpool_size = 8;







}