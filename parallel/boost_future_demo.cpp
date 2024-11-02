#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <future>

#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#include <boost/thread.hpp>

template<typename Func>
boost::unique_future<decltype(std::declval<Func>()())>
spawn_async(Func&& func){
    boost::promise<
        decltype(std::declval<Func>()())> p;
    auto res = p.get_future();
    boost::thread(
        [p=std::move(p),f=std::decay_t<Func>(func)]()
            mutable{
            try{
                std::cout << "set value\n";
                p.set_value_at_thread_exit(f());
            } catch(...){
                std::cout << "set exit\n";
                p.set_exception_at_thread_exit(std::current_exception());
            }
    });
    return res;
}

int make_int_slowly()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 42;
}

int main() {
  boost::unique_future<int> f1 = boost::async([]() { return 123; });
  boost::unique_future<std::string> f2 = f1.then([](boost::unique_future<int> f) { return std::to_string(f.get()) + " demo"; });

  boost::unique_future<int> fi = boost::async(make_int_slowly);
  boost::unique_future<int> fi2 = boost::async(make_int_slowly);

//   boost::wait_for_any(fi, fi2, f2);
  boost::wait_for_all(fi, fi2, f2);
  std::cout << f2.get() << "\n";
  std::cout << fi.get() << "\n";
  std::cout << fi2.get() << "\n";

  auto res = spawn_async(make_int_slowly)
    .then([](boost::unique_future<int> f) { return f.get() + 100; })
    .then([](boost::unique_future<int> f) { return "hello this is : " + std::to_string(f.get()); });
  std::cout << "res: " << res.get() << "\n"; 
}