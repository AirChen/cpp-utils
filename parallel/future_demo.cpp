#include <future>
#include <iostream>
#include <string>
#include <queue>
#include <deque>
#include <mutex>

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
}
// https://gitbookcpp.llfc.club/sections/cpp/concurrent/concpp07.html