#include <numeric>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>

int main() {
    std::list<int> l(10);
    std::iota(l.begin(), l.end(), -4);
 
    std::vector<std::list<int>::iterator> v(l.size());
    std::iota(v.begin(), v.end(), l.begin());
 
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
 
    std::cout << "Original contents of the list l:\t";
    for (const auto& n : l)
        std::cout << std::setw(2) << n << ' ';
    std::cout << '\n';
 
    std::cout << "Contents of l, viewed via shuffled v:\t";
    for (const auto i : v)
        std::cout << std::setw(2) << *i << ' ';
    std::cout << '\n';
    return 0;
}