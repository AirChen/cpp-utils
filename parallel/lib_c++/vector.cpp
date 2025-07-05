// vector.cpp
#include <vector>
#include <iostream>

int main() {
    std::vector<int> intVec1(5, 2011);
    intVec1.reserve(10); // --> capacity
    std::cout << intVec1.size() << '\n'; // 5
    std::cout << intVec1.capacity() << '\n'; // 10

    intVec1.resize(2);
    std::cout << intVec1.size() << '\n'; // 2
    std::cout << intVec1.capacity() << '\n'; // 10

    intVec1.shrink_to_fit(); // --> capacity
    std::cout << intVec1.capacity() << '\n'; // 5

    std::vector<int> intVec2(10);
    std::cout << intVec2.size() << '\n'; // 10

    std::vector<int> intVec3{10};
    std::cout << intVec3.size() << '\n'; // 1

    std::vector<int> intVec4{5, 2011};
    std::cout << intVec4.size() << '\n'; // 2

    // reserve 不填充数据，resize 填充数据
    // reserve 后调用 push 填充数据
    // resize 后可以直接用 [] 访问和设置
    return 0;
}