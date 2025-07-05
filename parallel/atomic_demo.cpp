#include <atomic>
#include <thread>
#include <iostream>
#include <type_traits>

struct FlagMutex {
    std::atomic_flag flag_{};
    void lock() {
        while (flag_.test_and_set(std::memory_order_acquire)) {
            flag_.wait(true, std::memory_order_relaxed);
        }
    }
    bool try_lock() {
        return !flag_.test_and_set(std::memory_order_acquire);
    }
    void unlock() {
        flag_.clear(std::memory_order_release);
        flag_.notify_one();
    }
};

struct SharedInstance {
    int data;

    void Increase() {
        data++;
    }

    void Decrease() {
        data--;
    }

    void Print() {
        std::cout << "data: " << data << "\n";
    }
};

int main() {
    SharedInstance instance;
    FlagMutex mutex;
    
    std::thread t1([&instance, &mutex]() {
        for (int i = 0; i < 100000; i++) {
            std::lock_guard<FlagMutex> lock(mutex);
            instance.Increase();
        } 
    });

    std::thread t2([&instance, &mutex]() {
        for (int i = 0; i < 100000; i++) {
            std::lock_guard<FlagMutex> lock(mutex);
            instance.Decrease();
        }
    });
    t1.join();
    t2.join();
    instance.Print();

    std::atomic<int> ai{2};
    int test_val = 3;
    int new_val = 4;
    std::cout << "is lock free: " << ai.is_lock_free() << " test_val: " << test_val << " new_val: " << new_val << "\n";
    bool ex = ai.compare_exchange_strong(test_val, new_val);
    std::cout << "ex: " << ex << " test_val: " << test_val << " new_val: " << new_val << "\n";
    ex = ai.compare_exchange_strong(test_val, new_val);
    std::cout << "ex: " << ex << " test_val: " << test_val << " new_val: " << new_val << "\n";

    ex = false;
    test_val = 3;
    int count = 0;
    while(!ex && test_val != new_val) {
        ex = ai.compare_exchange_weak(test_val, new_val);
        count++;
    }
    std::cout << "ex: " << ex << " test_val: " << test_val << " new_val: " << new_val << " count: " << count << "\n";

    std::atomic<int> ai2{2};
    auto c = ai2++;
    std::cout << "ai2 is value type : " << std::is_object<int>(c) << "\n";
    std::cout << "ai2 is value type : " << std::is_reference<int>(c) << "\n";
}