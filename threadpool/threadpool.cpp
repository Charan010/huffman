#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <array>
#include <condition_variable>

//shifting to int64 to prevent accidental int overflows.
using FrequencyTable = std::array<uint64_t, 256>;

struct Chunk {
    std::vector<char> data;
    int chunk_id;
};


class HuffmanCounter {
public:
    HuffmanCounter(size_t num_threads) : stop(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back(&HuffmanCounter::worker_loop, this);
        }
    }

    ~HuffmanCounter() {
        shutdown();
    }

    void process_file(const std::string& path, size_t chunk_size = 4 * 1024 * 1024) {
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            std::cerr << "Error: Could not open " << path << std::endl;
            return;
        }

        while (file) {
            Chunk chunk;
            chunk.data.resize(chunk_size);
            file.read(chunk.data.data(), chunk_size);
            size_t bytes_read = file.gcount();
            
            if (bytes_read == 0)
                break;

            if (bytes_read < chunk_size)
                chunk.data.resize(bytes_read);

            {
                std::lock_guard<std::mutex> lock(queue_mtx);
                jobs.push(std::move(chunk));

            }
            cv.notify_one();
        }
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(queue_mtx);
            if (stop) return;
            stop = true;
        }
        cv.notify_all();
        for (auto &t : workers) {
            if (t.joinable()) t.join();
        }
    }

private:

    void worker_loop() {
        while (true) {
            Chunk chunk;
            {
                std::unique_lock<std::mutex> lock(queue_mtx);
                cv.wait(lock, [this] { return stop || !jobs.empty(); });
                
                if (stop && jobs.empty())
                    return;

                chunk = std::move(jobs.front());
                jobs.pop();
            }

            FrequencyTable local_table = {0};
            for (unsigned char byte : chunk.data) {
                local_table[byte]++;
            }

            std::lock_guard<std::mutex> lock(global_mtx);
            for (size_t i = 0; i < 256; ++i)
                global_freq[i] += local_table[i];
            
        }
    }

    std::vector<std::thread> workers;
    std::queue<Chunk> jobs;
    std::mutex queue_mtx;
    std::condition_variable cv;
    bool stop;

    FrequencyTable global_freq = {0};
    std::mutex global_mtx;
};