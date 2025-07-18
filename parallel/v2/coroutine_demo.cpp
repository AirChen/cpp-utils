#include <iostream>
#include <chrono>
#include <future>
#include <coroutine>

using namespace std::chrono_literals;

auto foo()
{
  std::cout << "call foo\n";
    std::this_thread::sleep_for(3s);
    co_return 5;
}

auto bar()
{
  std::cout << "call bar\n";
  std::cout << "before foo\n";
  auto n = co_await std::async(foo);       // 挂起点
  std::cout << "after foo\n";
  co_return n;
}

int main()
{
  std::cout << "before bar\n";
  auto i = bar();
  std::cout << "after bar\n";
    i.wait();
  std::cout << "result = " << i.get().get();
}