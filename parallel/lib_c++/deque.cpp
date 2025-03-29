// deque.cpp
#include <deque>
#include <iostream>

struct MyInt{
MyInt(int i): myInt(i){};
int myInt;
};

int main() {
    std::deque<MyInt> myIntDeq;
    myIntDeq.push_back(MyInt(5));
    myIntDeq.emplace_back(1);

    std::cout << myIntDeq.size() << '\n'; // 2
    std::cout << std::endl;

    std::deque<int> intDeq;
    intDeq.assign({1, 2, 3});
    for (auto v: intDeq) std::cout << v << " "; // 1 2 3
    std::cout << std::endl;

    intDeq.insert(intDeq.begin(), 0);
    for (auto v: intDeq) std::cout << v << " "; // 0 1 2 3
    std::cout << std::endl;

    intDeq.insert(intDeq.begin()+4, 4);
    for (auto v: intDeq) std::cout << v << " "; // 0 1 2 3 4
    std::cout << std::endl;

    intDeq.insert(intDeq.end(), {5, 6, 7, 8, 9, 10, 11});
    for (auto v: intDeq) std::cout << v << " "; // 0 1 2 3 4 5 6 7 8 9 10 11
    std::cout << std::endl;

    for (auto revIt= intDeq.rbegin(); revIt != intDeq.rend(); ++revIt)
        std::cout << *revIt << " "; // 11 10 9 8 7 6 5 4 3 2 1 0
    std::cout << std::endl;

    intDeq.pop_back();
    for (auto v: intDeq) std::cout << v << " "; // 0 1 2 3 4 5 6 7 8 9 10
    std::cout << std::endl;

    intDeq.push_front(-1);
    for (auto v: intDeq) std::cout << v << " "; // -1 0 1 2 3 4 5 6 7 8 9 10
    std::cout << std::endl;
    return 0;
}