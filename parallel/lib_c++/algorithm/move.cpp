#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <list>
#include <thread>
#include <vector>
 
void f(int n)
{
    std::this_thread::sleep_for(std::chrono::seconds(n));
    std::cout << "thread " << n << " ended" << std::endl;
}
 
int main()
{
    std::vector<int> myVec{0, 1, 2, 3, 4, 5, 6, 7, 9};
    std::vector<int> myVec2(myVec.size());
    std::move(myVec.begin(), myVec.end(), myVec2.begin());
    for (auto v: myVec2) std::cout << v << " "; // 0 1 2 3 4 5 6 7 8 9
    std::cout << std::endl;
    for (auto v: myVec) std::cout << v << " ";
    std::cout << std::endl;

    std::string str{"abcdefghijklmnop"};
    std::string str2{"---------------------"};
    std::move_backward(str.begin(), str.end(), str2.end());
    std::cout << str2; // -----abcdefghijklmnop
    std::cout << std::endl;
    std::cout << str;
    std::cout << std::endl;

    // std::vector<std::jthread> v;
    // v.emplace_back(f, 1);
    // v.emplace_back(f, 2);
    // v.emplace_back(f, 3);
    // std::list<std::jthread> l;
 
    // // copy() would not compile, because std::jthread is noncopyable
    // std::move(v.begin(), v.end(), std::back_inserter(l));
}