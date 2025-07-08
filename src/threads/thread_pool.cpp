#include <threads/thread_pool.hpp>

ThreadPool::ThreadPool(size_t thread_count, size_t max_number_tasks)
    : max_number_tasks(max_number_tasks)
{
    for (size_t i = 0; i < thread_count; ++i) {
        workers.emplace_back([this]() { this->worker(); });
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock lock(queue_mutex);
        tasks.emplace(std::move(task));
    }
    condition.notify_one();
}

void ThreadPool::shutdown() {
    stop.store(true);
    condition.notify_all();

    for (auto &thread : workers) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

bool ThreadPool::available() const {
    return tasks.size() < max_number_tasks;
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock lock(queue_mutex);
            condition.wait(lock, [this]() { return stop || !tasks.empty(); });

            if (stop && tasks.empty()) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
    }
}
