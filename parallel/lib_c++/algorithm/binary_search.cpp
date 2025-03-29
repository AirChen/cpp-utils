// binarySearch.cpp
#include <algorithm>
#include <iostream>
using namespace std;
bool isLessAbs(int a, int b){
    return abs(a) < abs(b);
}

int main() {
    vector<int> vec{-3, 0, -3, 2, -3, 5, -3, 7, -0, 6, -3, 5,
    -6, 8, 9, 0, 8, 7, -7, 8, 9, -6, 3, -3, 2};
    sort(vec.begin(), vec.end(), isLessAbs);
    for (auto v: vec) cout << v << " ";
    std::cout << std::endl;
    // 0 0 0 2 2 -3 -3 -3 -3 -3 3 -3 5 5 -6 -6 6 7 -7 7 8 8 8 9 9
    cout << binary_search(vec.begin(), vec.end(), -5, isLessAbs); // true
    std::cout << std::endl;
    cout << binary_search(vec.begin(), vec.end(), 5, isLessAbs); // true
    std::cout << std::endl;
    
    auto pair= equal_range(vec.begin(), vec.end(), 3, isLessAbs);
    cout << distance(vec.begin(), pair.first); // 5
    std::cout << std::endl;

    cout << distance(vec.begin(), pair.second) - 1; // 11
    std::cout << std::endl;
    
    for (auto threeIt= pair.first;threeIt != pair.second; ++threeIt)
        cout << *threeIt << " "; // -3 -3 -3 -3 -3 3 -3
    std::cout << std::endl;
    return 0;
}