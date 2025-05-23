// merge.cpp
#include <algorithm>
#include <iostream>

int main() {
    std::vector<int> vec1{1, 1, 4, 3, 5, 8, 6, 7, 9, 2};
    std::vector<int> vec2{1, 2, 3};
    std::sort(vec1.begin(), vec1.end());
    std::vector<int> vec(vec1);
    vec1.reserve(vec1.size() + vec2.size());
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    for (auto v: vec1) std::cout << v << " "; // 1 1 2 3 4 5 6 7 8 9 1 2 3
    std::cout << "\n";
    
    std::inplace_merge(vec1.begin(), vec1.end()-vec2.size(), vec1.end());
    for (auto v: vec1) std::cout << v << " "; // 1 1 1 2 2 3 3 4 5 6 7 8 9
    std::cout << "\n";
    
    vec2.push_back(10);
    for (auto v: vec) std::cout << v << " "; // 1 1 2 3 4 5 6 7 8 9
    std::cout << "\n";
    
    for (auto v: vec2) std::cout << v << " "; // 1 2 3 10
    std::cout << "\n";
    
    std::vector<int> res;
    std::set_symmetric_difference(vec.begin(), vec.end(), vec2.begin(), vec2.end(),
    std::back_inserter(res));
    for (auto v : res) std::cout << v << " "; // 1 4 5 6 7 8 9 10
    std::cout << "\n";
    
    res= {};
    std::set_union(vec.begin(), vec.end(), vec2.begin(), vec2.end(),
    std::back_inserter(res));
    for (auto v : res) std::cout << v << " "; // 1 1 2 3 4 5 6 7 8 9 10
    std::cout << "\n";
    
    return 0;
}