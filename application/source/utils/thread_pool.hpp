#pragma once

#include <functional>
#include <future>
#include <queue>
#include <thread>

class ThreadPool {
    std::mutex              mutex;
    std::condition_variable condition;

    std::vector<std::thread>               workers;
    std::queue<std::packaged_task<void()>> tasks;

    bool stopped;

public:
    explicit ThreadPool(std::size_t threads);
    ~ThreadPool();

    template <class F, class... Args>
    auto AddTask(F&& func, Args&& ...args);
};

ThreadPool::ThreadPool(std::size_t threads) : stopped(false) {
    for (std::size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::packaged_task<void()> task;

                {
                    std::unique_lock lock(mutex);
                    this->condition.wait(lock, [this]() {
                        return this->stopped || !this->tasks.empty();
                    });

                    if (this->stopped || this->tasks.empty()) {
                        return;
                    }

                    task = std::move(this->tasks.front());
                    tasks.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock lock(mutex);
        stopped = true;
    }
    condition.notify_all();

    for (auto& thread : workers) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

template <class F, class... Args>
auto ThreadPool::AddTask(F&& func, Args&& ...args) {
    using return_type = std::invoke_result_t<F, Args...>;
    std::packaged_task<return_type()> task(std::bind(std::forward<F>(func), std::forward<Args>(args)...));

    std::future<return_type> result = task.get_future();
    {
        std::unique_lock lock(mutex);
        if (stopped) {
            throw std::runtime_error("Trying to add task to stopped pool");
        }
        tasks.emplace(std::move(task));
    }
    condition.notify_one();
    return result;
}