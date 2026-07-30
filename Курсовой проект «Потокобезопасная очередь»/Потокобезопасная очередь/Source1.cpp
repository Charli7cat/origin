#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <chrono>
#include <atomic>
#include <random>

template<typename T>
class safe_queue {
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
    std::atomic<bool> stop_{ false };

public:
    safe_queue() = default;
    ~safe_queue() = default;

    safe_queue(const safe_queue&) = delete;
    safe_queue& operator=(const safe_queue&) = delete;

    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(value));
        }
        cond_var_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this] {
            return !queue_.empty() || stop_.load();
            });

        if (stop_.load() && queue_.empty()) {
            return T();
        }

        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    void stop() {
        stop_.store(true);
        cond_var_.notify_all();
    }
};

class thread_pool {
private:
    std::vector<std::thread> workers_;
    safe_queue<std::function<void()>> tasks_;
    std::atomic<bool> stop_{ false };

    void work() {
        while (!stop_.load()) {
            auto task = tasks_.pop();
            if (task) {
                try {
                    task();
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception in task: " << e.what() << std::endl;
                }
                catch (...) {
                    std::cerr << "Unknown exception in task" << std::endl;
                }
            }
        }
    }

public:
    thread_pool() {
        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) {
            num_threads = 4;
        }
        std::cout << "Creating thread pool with " << num_threads << " threads" << std::endl;

        for (unsigned int i = 0; i < num_threads; ++i) {
            workers_.emplace_back(&thread_pool::work, this);
        }
    }

    ~thread_pool() {
        stop_.store(true);
        tasks_.stop();

        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        std::cout << "Thread pool destroyed" << std::endl;
    }

    thread_pool(const thread_pool&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;

    template<typename F>
    auto submit(F&& f) -> std::future<typename std::invoke_result_t<F>> {
        using return_type = typename std::invoke_result_t<F>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::forward<F>(f)
        );

        auto future = task->get_future();
        tasks_.push([task]() { (*task)(); });

        return future;
    }

    void submit(const std::function<void()>& task) {
        tasks_.push(task);
    }

    size_t pending_tasks() const {
        return tasks_.size();
    }

    bool is_stopped() const {
        return stop_.load();
    }
};

void test_function_1() {
    std::cout << "Function 1 executed by thread "
        << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void test_function_2() {
    std::cout << "Function 2 executed by thread "
        << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

void test_function_3() {
    std::cout << "Function 3 executed by thread "
        << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
}

void test_function_with_params(int id, const std::string& name) {
    std::cout << "Function " << id << " (" << name << ") executed by thread "
        << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

int test_function_return_value(int value) {
    std::cout << "Function returning " << value * 2 << " executed by thread "
        << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return value * 2;
}

void demonstrate_thread_pool() {
    thread_pool pool;

    std::cout << "\n=== Test 1: Simple tasks with std::function ===" << std::endl;

    for (int i = 0; i < 10; ++i) {
        pool.submit([i]() {
            std::cout << "Simple task " << i << " by thread "
                << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            });
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\n=== Test 2: Tasks with return values ===" << std::endl;

    std::vector<std::future<int>> futures;
    for (int i = 0; i < 5; ++i) {
        auto future = pool.submit([i]() -> int {
            std::cout << "Return task " << i << " by thread "
                << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return i * i;
            });
        futures.push_back(std::move(future));
    }

    for (size_t i = 0; i < futures.size(); ++i) {
        try {
            int result = futures[i].get();
            std::cout << "Task " << i << " returned: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error getting result: " << e.what() << std::endl;
        }
    }

    std::cout << "\n=== Test 3: Different function signatures ===" << std::endl;

    pool.submit(test_function_1);
    pool.submit(test_function_2);
    pool.submit(test_function_3);

    pool.submit([&pool]() {
        for (int i = 0; i < 3; ++i) {
            pool.submit([i]() {
                test_function_with_params(i, "Task");
                });
        }
        });

    auto result_future = pool.submit([]() -> int {
        return test_function_return_value(42);
        });

    try {
        int result = result_future.get();
        std::cout << "Return task result: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "\n=== Final pool status ===" << std::endl;
    std::cout << "Pending tasks: " << pool.pending_tasks() << std::endl;
}

int main() {
    try {
        demonstrate_thread_pool();

        std::cout << "\n=== Press Enter to exit ===" << std::endl;
        std::cin.get();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}