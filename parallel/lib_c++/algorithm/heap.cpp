// heap.cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>

void test_pop_ordered() {
    std::vector<int> test_datas{
        1, 1, 1, 1, 2, 2, 2, 3
    };
    int k = 2;

    std::unordered_map<int, int> freq_map;
    std::vector<std::pair<int, int>> heap_vec;
    for (const auto& n : test_datas) {
        freq_map[n]++;
    }

    std::make_heap(heap_vec.begin(), heap_vec.end());
    for (const auto& [key, freq] : freq_map) {
        heap_vec.push_back({key, freq});
        std::push_heap(heap_vec.begin(), heap_vec.end(), [](const auto& lp, const auto& rp) {
            return lp.second < rp.second;
        });
    }

    for (int i = 0; i < k; i++) {
        std::pop_heap(heap_vec.begin(), heap_vec.end(), [](const auto& lp, const auto& rp) {
            return lp.second < rp.second;
        });

        const auto& max_element = heap_vec.back();
        heap_vec.pop_back();
        std::cout << " [" << max_element.first << ": " << max_element.second << "] -- ";
    }
    std::cout << "\n";
}

void normal_case() {
    std::vector<int> vec{4, 3, 2, 1, 5, 6, 7, 9, 10};
    std::make_heap(vec.begin(), vec.end());
    for (auto v: vec) std::cout << v << " "; // 10 9 7 4 5 6 2 3 1
    std::cout << "\n";
    
    std::cout << std::is_heap(vec.begin(), vec.end()); // true
    std::cout << "\n";
    
    vec.push_back(100);
    std::cout << std::is_heap(vec.begin(), vec.end()); // false
    std::cout << "\n";
    
    std::cout << *std::is_heap_until(vec.begin(), vec.end()); // 100
    std::cout << "\n";
    
    for (auto v: vec) std::cout << v << " "; // 10 9 7 4 5 6 2 3 1 100
    std::cout << "\n";
    
    std::push_heap(vec.begin(), vec.end());
    std::cout << std::is_heap(vec.begin(), vec.end()); // true
    std::cout << "\n";
    
    for (auto v: vec) std::cout << v << " "; // 100 10 7 4 9 6 2 3 1 5
    std::cout << "\n";
    
    std::pop_heap(vec.begin(), vec.end());
    for (auto v: vec) std::cout << v << " "; // 10 9 7 4 5 6 2 3 1 100
    std::cout << "\n";
    
    std::cout << *std::is_heap_until(vec.begin(), vec.end()); // 100
    std::cout << "\n";
    
    vec.resize(vec.size()-1);
    std::cout << std::is_heap(vec.begin(), vec.end()); // true
    std::cout << "\n";
    std::cout << vec.front() << '\n'; // 10
    std::cout << "\n";
}

int main() {
    test_pop_ordered();
    return 0;
}