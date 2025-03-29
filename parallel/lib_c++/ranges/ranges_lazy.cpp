// rangesLazy.cpp
#include <ranges>
#include <iostream>

bool isPrime(int i) {
    for (int j=2; j*j <= i; ++j){
        if (i % j == 0) return false;
    }
    return true;
}

int main() {
    auto odd = [](int i){ return i % 2 == 1; };
    std::cout << "20 prime numbers starting with 1000000: " << '\n';
    for (int i: std::views::iota(1000000) | std::views::filter(odd)
        | std::views::filter(isPrime)
        | std::views::take(20)) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    return 0;
}