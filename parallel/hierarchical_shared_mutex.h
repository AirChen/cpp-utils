#include <shared_mutex>
#include <stdexcept>
#include <climits>
class hierarchical_shared_mutex
{
    std::shared_mutex internal_mutex;
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
        previous_hierarchy_value=this_thread_hierarchy_value;
        this_thread_hierarchy_value=hierarchy_value;
    }
public:
    explicit hierarchical_shared_mutex(unsigned long value):
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

    void lock_shared() {
        internal_mutex.lock_shared();
    }

    bool try_lock_shared() {
        return internal_mutex.try_lock_shared();
    }

    void unlock_shared() {
        internal_mutex.unlock_shared();
    }
};
thread_local unsigned long
    hierarchical_shared_mutex::this_thread_hierarchy_value(ULONG_MAX);
