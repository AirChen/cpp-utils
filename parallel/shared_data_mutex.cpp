#include <mutex>
#include <stdexcept>
#include <climits>
#include <thread>

class hierarchical_mutex
{
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;

    void check_for_hierarchy_violation()
    {
        if(this_thread_hierarchy_value <= hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }

    void update_hierarchy_value()
    {
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }

public:
    explicit hierarchical_mutex(unsigned long value):
        hierarchy_value(value),
        previous_hierarchy_value(0)
    {}
    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }
    void unlock()
    {
        this_thread_hierarchy_value=previous_hierarchy_value;
        internal_mutex.unlock();
    }
    bool try_lock()
    {
        check_for_hierarchy_violation();
        if(!internal_mutex.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }
};
thread_local unsigned long
    hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);       

hierarchical_mutex low_level_mutex(2000);
hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex other_level_mutex(6000);

void process_low_level_func() {}
void low_level_staff() {
    std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
    process_low_level_func();
}

void process_high_level_func() {}
void high_level_staff() {
    std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
    low_level_staff();
    process_high_level_func();
}

void process_other_level_func() {}
void other_level_staff() {
    std::lock_guard<hierarchical_mutex> lk(other_level_mutex);
    high_level_staff(); // error lock
    process_other_level_func();
}

class SomeSingleSource {
private:
    void setup_sql_connect(){};
    void make_big_memory(){};
    void exec_sql_lang(){};
    void calc_part_memory(){};
    std::once_flag source_inital_flag;
public:
    void send_msg() {
        std::call_once(source_inital_flag, [&](){
            this->setup_sql_connect();
            this->make_big_memory();
        });
        calc_part_memory();
        exec_sql_lang();
    }
};

int main()
{
    std::thread thread_a(high_level_staff);
    std::thread thread_b(other_level_staff);

    thread_a.join();
    thread_b.join();

    SomeSingleSource source;
    std::thread thread_c([&source](){
        source.send_msg();
    });
    std::thread thread_d([&source](){
        source.send_msg();
    });

    thread_c.join();
    thread_d.join();
}
