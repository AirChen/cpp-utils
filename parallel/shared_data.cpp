#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

struct toy_obj {
    std::mutex m;

    int id;
    double price;
    double weight;
    bool owned{false};

    void get_toy() {
        owned = true;
    }

    void abundon_toy() {
        owned = false;
    }
};

struct shared_toys {
    toy_obj car_toy_;
    toy_obj fly_toy_;
};

class dead_lock_demo1 {
    shared_toys toys;
public:
    void process() {
        std::thread child_Tom_thread([&](){
            // std::lock_guard lk(toys.car_toy_.m);
            // std::lock_guard tk(toys.fly_toy_.m);
            std::scoped_lock slk(toys.car_toy_.m, toys.fly_toy_.m);

            toys.car_toy_.get_toy();
            toys.fly_toy_.get_toy();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            toys.car_toy_.abundon_toy();
            toys.fly_toy_.abundon_toy();
        });
        std::thread child_John_thread([&](){
            // std::lock_guard lk(toys.car_toy_.m);
            // std::lock_guard tk(toys.fly_toy_.m);
            std::scoped_lock slk(toys.car_toy_.m, toys.fly_toy_.m);

            toys.car_toy_.get_toy();
            toys.fly_toy_.get_toy();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            toys.car_toy_.abundon_toy();
            toys.fly_toy_.abundon_toy();
        });

        child_John_thread.join();
        child_Tom_thread.join();
    }
};

void thread_do_some_thing() {};
void thread_param_with_th(std::thread& th) {
    th = std::thread(thread_do_some_thing);
}

void dead_lock_demo2() {
    std::thread thread_a;
    std::thread thread_b([&](){
       thread_a.join(); 
    });
    thread_a = std::thread([&](){
        thread_b.join();
    });

    std::cout << "not dead lock demo2\n";
}

int main() {
    dead_lock_demo1 demo1;
    for (int i = 0; i < 100; ++i) {
        demo1.process();
    }
    std::cout << "not dead lock \n";

    dead_lock_demo2();
    return 0;
}