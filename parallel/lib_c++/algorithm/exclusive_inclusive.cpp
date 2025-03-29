#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
 
int main()
{
    std::vector data{3, 1, 4, 1, 5, 9, 2, 6};

    // std::inclusive_scan(data.begin(), data.end(), data.begin());
    // for (const auto& i : data) std::cout << i << " ";
    // std::cout << "\n";
 
    auto times_10 = [](int x) { return x * 10; };
 
    std::cout << "10 times exclusive sum: ";
    std::transform_exclusive_scan(data.begin(), data.end(),
                                  std::ostream_iterator<int>(std::cout, " "),
                                  0, std::plus<int>{}, times_10);
    std::cout << "\n10 times inclusive sum: ";
    std::transform_inclusive_scan(data.begin(), data.end(),
                                  std::ostream_iterator<int>(std::cout, " "),
                                  std::plus<int>{}, times_10);
    std::cout << '\n';
}

// similar to std::partial_sum, excludes the ith input element from the ith sum
// similar to std::partial_sum, includes the ith input element in the ith sum