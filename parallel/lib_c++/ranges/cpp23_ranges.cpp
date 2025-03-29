// cpp23Ranges.cpp
#include <ranges>
#include <generator>
#include <iostream>

std::generator<int> fib() {
    co_yield 0; // 1
    auto a = 0;
    auto b = 1;
    for(auto n : std::views::iota(0)) {
        auto next = a + b;
        a = b;
        b = next;
        co_yield next; // 2
    }
}

int main() {
    std::vector vec = {1, 2, 3, 4};
    for (auto i : vec | std::views::adjacent<2>) {
        std::cout << '(' << i.first << ", " << i.second << ") "; // (1, 2) (2, 3) (3, 4)
    }
    std::cout << "\n";

    for (auto i : vec | std::views::adjacent_transform<2>(std::multiplies())) {
        std::cout << i << ' '; // 2 6 12
    }
    std::cout << "\n";

    std::print("{}\n", vec | std::views::chunk(2)); // [[1, 2], [3, 4],
    std::cout << "\n";
    std::print("{}\n", vec | std::views::slide(2)); // [[1, 2], [2, 3], [3, 4]
    std::cout << "\n";

    for (auto i : vec | std::views::slide(2)) {
        std::cout << '[' << i[0] << ", " << i[1] << "] "; // [1, 2] [2, 3] [3, 4] [4, 5]
    }
    std::cout << "\n";

    std::vector vec2 = {1, 2, 3, 0, 5, 2};
    std::print("{}\n", vec2 | std::views::chunk_by(std::ranges::less_equal{}));
    std::cout << "\n";
    // [[1, 2, 3], [0, 5], [2]]

    for (auto i : vec | std::views::slide(2)) {
        std::cout << '[' << i[0] << ", " << i[1] << "] "; // [1, 2] [2, 3] [3, 4] [4, 5]
    }
    std::cout << "\n";

    for (auto f : fib() | std::views::take(10)) { // 3
        std::cout << f << " "; // 0 1 1 2 3 5 8 13 21 34
    }
    std::cout << "\n";

    return 0;
}