#include <list>
#include <algorithm>
#include <future>
#include <thread>
#include <mutex>
#include <deque>

//todo: thread_pool
class simple_thread_pool {
public:
    simple_thread_pool(int n): th_n_(n) {
        for (int i = 0; i < th_n_; ++i) {
            std::thread th([this]() {
                while (true) {
                    std::packaged_task<void()> task;
                    {
                        std::unique_lock lk(mx_);
                        empty_wait_cv_.wait(lk, [this]{ return !task_queue_.empty();});
                        task = std::move(task_queue_.front());
                        task_queue_.pop_front();
                        running_task_num_++;
                    }
                    
                    task();
                    running_task_num_--;
                    if (running_task_num_ == 0) {
                        finished_wait_cv_.notify_one();
                    }
                }
            });
            th.detach();
        }
    }

    void schedule(const std::function<void()>& func) {
        std::packaged_task<void()> pt(func);
        std::lock_guard lk(mx_);
        task_queue_.emplace_back(pt);
        empty_wait_cv_.notify_all();
    }

    void sync() {
        std::unique_lock lk(mx_);
        finished_wait_cv_.wait(lk, [this]{ running_task_num_ != 0; }); 
    }
private:
    int th_n_;
    std::deque<std::packaged_task<void()>> task_queue_;
    std::mutex mx_;
    std::condition_variable empty_wait_cv_;
    std::condition_variable finished_wait_cv_;
    std::atomic<int> running_task_num_{0};
    std::atomic<bool> cancel_and_empty_pool_{false};
};


template<typename F,typename A>
std::future<std::result_of<F(A&&)>::type>
spawn_task(F&& f,A&& a)
{
    typedef std::result_of<F(A&&)>::type result_type;
    std::packaged_task<result_type(A&&)>
        task(std::move(f));
    std::future<result_type> res(task.get_future());
    std::thread t(std::move(task),std::move(a));// thread_pool
    t.detach();
    return res;
}

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if(input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(),input,input.begin());
    T const& pivot=*result.begin();
    auto divide_point=std::partition(input.begin(),input.end(),
        [&](T const& t){return t<pivot;});
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(),input,input.begin(),
        divide_point);
    std::future<std::list<T> > new_lower(
        std::async(&parallel_quick_sort<T>,std::move(lower_part)));// todo: spawn_task 
    auto new_higher(
        parallel_quick_sort(std::move(input)));
    result.splice(result.end(),new_higher);
    result.splice(result.begin(),new_lower.get());
    return result;
}
