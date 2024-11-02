#include <future>
#include <iostream>
#include <string>
#include <queue>
#include <deque>
#include <mutex>
#include <thread>
#include <algorithm>

int some_staff_process() {
    return 42;
}

void process_other() {}

struct X
{
    void foo(int,std::string const&) {};
    std::string bar(std::string const&) {
        return "hello";
    };
};

struct Y
{
    double operator()(double) {
        return 0.1;
    }
};

class move_only
{
public:
    move_only() {};
    move_only(move_only&&) {};
    move_only(move_only const&) = delete;
    move_only& operator=(move_only&&) {};
    move_only& operator=(move_only const&) = delete;
    void operator()() {};
};

// gui demo
std::deque<std::packaged_task<void()>> task_queue;
std::mutex gui_mutex;

bool gui_shutdown_message_received() {
    return false;
}
void get_and_process_gui_message() {}

void gui_thread() {
    while (!gui_shutdown_message_received()) {
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard lk(gui_mutex);
            std::packaged_task<void()> task = std::move(task_queue.front());
            task_queue.pop_front();
        }
        task();
    }
}

template<typename Func>
std::future<Func> push_gui_task(Func& f) {
    std::packaged_task<void()> task(f);
    std::future<Func> res = task.get_future();
    std::lock_guard lk(gui_mutex);
    task_queue.push_back(task);
    return res;
}

void compute(std::promise<int> p) {
    int result = 42; // 模拟计算结果
    p.set_value(result); // 设置promise的值

    p.set_exception(std::exception_ptr(nullptr));
}

#include <type_traits>

template <typename T>
class future_chain {
private:
    std::future<T> future_;

public:
    future_chain() = default;
    
    explicit future_chain(std::future<T>&& future) : future_(std::move(future)) {}

    // Schedule the first task
    template <typename F>
    auto Schedule(F&& func) {
        using result_type = typename std::result_of<F()>::type;
        future_ = std::async(std::launch::async, std::forward<F>(func));
        return future_chain<result_type>(std::move(future_));
    }

    // Chain subsequent tasks
    template <typename F>
    auto then(F&& func) {
        using result_type = typename std::result_of<F(T)>::type;
        auto previous = std::move(future_);
        
        auto future = std::async(std::launch::async, [prev = std::move(previous), f = std::forward<F>(func)]() mutable {
            // prev.wait();  // Wait for previous task
            // return f();   // Execute next task
            return f(prev.get());
        });

        return future_chain<result_type>(std::move(future));
    }

    // Wait for the result
    T get() {
        return future_.get();
    }
};

// Helper function to create a future_chain
template <typename F>
auto make_future_chain(F&& func) {
    using result_type = typename std::result_of<F()>::type;
    return future_chain<result_type>().Schedule(std::forward<F>(func));
}

template<typename T>
class future_set {
private:
    std::vector<std::future<T>> futures_;

public:
    // 添加future到集合
    void add(std::future<T>&& future) {
        futures_.push_back(std::move(future));
    }

    // 等待所有future完成
    std::vector<T> wait_all() {
        std::vector<T> results;
        results.reserve(futures_.size());
        
        for (auto& fut : futures_) {
            results.push_back(fut.get());
        }
        
        return results;
    }

    // 等待任意一个future完成
    std::pair<size_t, T> wait_any() {
        while (true) {
            for (size_t i = 0; i < futures_.size(); ++i) {
                if (futures_[i].valid()) {
                    auto status = futures_[i].wait_for(std::chrono::milliseconds(0));
                    if (status == std::future_status::ready) {
                        return {i, futures_[i].get()};
                    }
                }
            }
            std::this_thread::yield();
        }
    }
};

// 辅助函数：创建future_set并等待所有future完成
template<typename T>
std::vector<T> when_all(std::vector<std::future<T>>&& futures) {
    future_set<T> set;
    for (auto& fut : futures) {
        set.add(std::move(fut));
    }
    return set.wait_all();
}

// 辅助函数：创建future_set并等待任意一个future完成
template<typename T>
std::pair<size_t, T> when_any(std::vector<std::future<T>>&& futures) {
    future_set<T> set;
    for (auto& fut : futures) {
        set.add(std::move(fut));
    }
    return set.wait_any();
}

int main() {
    std::future<int> fut = std::async(some_staff_process);
    process_other();
    std::cout << "get result: " << fut.get() << "\n";
    
    X x;
    auto f1=std::async(&X::foo,&x,42,"hello");
    auto f2=std::async(&X::bar,x,"goodbye");
    
    Y y;
    auto f3=std::async(Y(),3.141);
    auto f4=std::async(std::ref(y),2.718);
    auto f5=std::async(move_only());

    std::promise<int> p; // 创建promise对象
    std::future<int> f = p.get_future(); // 获取关联的future对象
    std::thread t(compute, std::move(p)); // 启动线程并传递promise

    int result = f.get(); // 获取结果
    std::cout << "Result: " << result << std::endl;

    t.join(); // 等待线程结束

    auto chain = make_future_chain([]() {
        std::cout << "Task 1 running in thread " << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task 1 completed\n";
        return 1;
    }).then([](int prev) {
        std::cout << "Task 2 running in thread " << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task 2 completed\n";
        return prev + 2;
    }).then([](int prev) {
        std::cout << "Task 3 running in thread " << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task 3 completed\n";
        return prev + 3;
    });

    // Wait for all tasks to complete and get final result
    std::cout << "Final result: " << chain.get() << std::endl;
}
// https://gitbookcpp.llfc.club/sections/cpp/concurrent/concpp07.html