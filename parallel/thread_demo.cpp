#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <algorithm>

void single_func() {
    std::cout << "single_func \n";
}

void thread_task_demo1(int* val) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    (*val)++;
}

struct thread_func {
    std::function<void()> loc_func_;
    thread_func(const std::function<void()>& func) {
        loc_func_ = func;
    }

    void operator()() {
        for (int i = 0; i < 5; ++i) {
            loc_func_();
        }
    }
};

class thread_guard {
    std::thread &t;
public:
    explicit thread_guard(std::thread& th): t(th) {}
    ~thread_guard() {
        if (t.joinable()) {
            std::cout << "join in thread_guard \n";
            t.join();
        }
    }
};

class scope_guard {
    std::thread t;
public:
    explicit scope_guard(std::thread& th): t(std::move(th)) {
        if (!t.joinable()) {
            throw std::runtime_error("thread not joinable!");
        }
        std::cout << "scope thread id: " << t.get_id() << "\n";
    }
    ~scope_guard() {
        std::cout << "join in scope_guard \n";
        t.join();
    }
};

void move_test_job1() {
    std::cout << "in move job 1 \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "out move job 1 \n";
}

void move_test_job2() {
    std::cout << "in move job 2 \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "out move job 2 \n";
}

void do_homework(int i) {
    std::cout << "do homework " << i << " thread id " << std::this_thread::get_id() <<  "\n";
}

class common_test_cls {
    int v_{0};
public:
    void process_sub() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "process : " << v_++ << "\n";
    }
};

int main() {
    int test_val = 0;
    std::thread demo1(thread_task_demo1, &test_val);
    // thread_guard tg(demo1);

    thread_func func(single_func);
    std::thread demo2(func);
    demo2.detach();

    std::thread demo3([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "echo in demo3\n";
    });
    thread_guard g(demo3);
    scope_guard d(demo3);

    demo1.join();
    std::cout << test_val << "\n";

    std::thread t1(move_test_job1);
    std::thread t2(std::move(t1));

    t1 = std::thread(move_test_job2);
    std::thread t3;
    t3 = std::move(t2);
    // t1 = std::move(t3);// t1 有任务正在执行 move 过去 会导致 libc++abi: terminating

    scope_guard s1(t1);
    thread_guard s2(t2);
    thread_guard s3(t3);

    // jthread https://en.cppreference.com/w/cpp/compiler_support/20  compiler support
    // It has the same general behavior as std::thread, except that jthread automatically rejoins on destruction, and can be cancelled/stopped in certain situations.
    // std::cout << "hardware concurrency: " << std::jthread::hardware_concurrency() << "\n";
    // std::jthread jt([&](){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // });

    std::vector<std::thread> threads;
    common_test_cls common_obj;
    for (int i = 0; i < 20; ++i) {
        threads.emplace_back(std::thread(&common_test_cls::process_sub, &common_obj));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "done in main job: " << std::this_thread::get_id() << "\n";
    return 0;
}