#include <array>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
 
void println(auto comment, const auto& sequence)
{
    std::cout << comment;
    for (const auto& n : sequence)
        std::cout << n << ' ';
    std::cout << '\n';
};
 
int main()
{
    // Default implementation - the difference between two adjacent items
    std::vector v{4, 6, 9, 13, 18, 19, 19, 15, 10};
    println("Initially, v = ", v);
    std::adjacent_difference(v.begin(), v.end(), v.begin());
    println("Modified v = ", v);
 
    // Fibonacci
    std::array<int, 10> a {1};
    std::adjacent_difference(std::begin(a), std::end(a),
                             std::next(std::begin(a)), std::plus<>{});
    println("Fibonacci, a = ", a);
}