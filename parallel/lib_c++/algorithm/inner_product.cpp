#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

struct CustomInc {
    int operator()(int a, int b) {
        return (a + b) * (a - b);
    } 
};

int main()
{
    std::vector<int> a{0, 1, 2, 3, 4};
    std::vector<int> b{5, 4, 2, 3, 1};
 
    int r1 = std::inner_product(a.begin(), a.end(), b.begin(), 0);
    std::cout << "Inner product of a and b: " << r1 << '\n';
 
    int r2 = std::inner_product(a.begin(), a.end(), b.begin(), 0,
                                std::plus<>(), std::equal_to<>());
    std::cout << "Number of pairwise matches between a and b: " <<  r2 << '\n';
    
    int r3 = std::inner_product(a.begin(), a.end(), b.begin(), 0,
                                std::plus<>(), CustomInc());
    std::cout << "CustomInc val acc a and b: " <<  r3 << '\n';
}