#include <mutex>
#include <thread>
#include <queue>
#include <memory>

struct test_data{};
bool more_data_to_prepare() {
    return false;
}

test_data prepare_data() {
    return test_data();
}

void process_data(const test_data&) {}
bool is_last_data() {
    return true;
}

std::mutex lock_mutex;
std::condition_variable condition_var;
std::queue<test_data> test_queue;

void data_prepare_thead_func() {
    while (!more_data_to_prepare()) {
        std::lock_guard lk(lock_mutex);
        test_data d = prepare_data();
        test_queue.push(d);
        condition_var.notify_one();
        // condition_var.notify_all(); // if process thread is many, not single
    }
}

void data_process_thread_func() {
    while (true)
    {
        std::unique_lock lk(lock_mutex);
        condition_var.wait(lk, []{return !test_queue.empty();});
        test_data d = test_queue.front();
        test_queue.pop();
        lk.unlock();

        process_data(d);
        if (is_last_data()) {
            break;
        }
    }
}

template<typename T>
class threasafe_queue {
private:
    mutable std::mutex mx_;
    std::condition_variable condition_var_;
    std::queue<T> queue_;

public:
    void push(const T& d) {
        std::lock_guard lk(mx_);
        queue_.push(d);
        condition_var_.notify_all();
    }

    bool try_pop(T& out_d) {
        std::lock_guard lk(mx_);
        if (queue_.empty()) {
            return false;
        }

        out_d = queue_.front();
        queue_.pop();
        return true;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard lk(mx_);
        if (queue_.empty())
            return nullptr;

        std::shared_ptr<T> res(queue_.front());
        queue_.pop();
        return res;
    }

    void wait_and_pop(T& out_d) {
        std::unique_lock lk(mx_);
        condition_var_.wait(lk, [this]{return !queue_.empty();});
        out_d = queue_.front();
        queue_.pop(); 
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock lk(mx_);
        condition_var_.wait(lk, [this]{return !queue_.empty();});
        std::shared_ptr<T> res(queue_.front());
        queue_.pop(); 
        return res;
    }

    bool empty() const {
        std::lock_guard lk(mx_);
        return queue_.empty();
    }
};

threasafe_queue<test_data> ts_queue;
void data_prepare_thead_func_v2() {
    while (!more_data_to_prepare()) {
        ts_queue.push(prepare_data());
    }
}

void data_process_thread_func_v2() {
    while (true)
    {
        test_data d;
        ts_queue.wait_and_pop(d);
        process_data(d);
        if (is_last_data()) {
            break;
        }
    }
}

int main() {
    // std::thread thread_a(data_prepare_thead_func);
    // std::thread thread_b(data_process_thread_func);
    std::thread thread_a(data_prepare_thead_func_v2);
    std::thread thread_b(data_process_thread_func_v2);

    thread_a.join();
    thread_b.join();
}