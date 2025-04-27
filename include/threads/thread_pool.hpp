#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t thread_count, size_t max_number_tasks);
    ~ThreadPool();

    void enqueue(std::function<void()> task);
    void shutdown();
    bool available() const;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    size_t max_number_tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop{false};

    void worker();
};
