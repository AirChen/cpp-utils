// printSpan.cpp
#include <iostream>
#include <span> // c++20

void printMe(std::span<int> container) {
    std::cout << "container.size(): " << container.size() << '\n';
    for(auto e : container) std::cout << e << ' ';
    std::cout << "\n\n";
}

int main() {
    std::cout << '\n';
    int arr[]{1, 2, 3, 4};
    printMe(arr); // (1)

    std::vector vec0{1, 2, 3, 4, 5};
    printMe(vec0); // (2)

    std::vector vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printMe(vec);
    
    std::span span1(vec); // (1)
    std::span span2{span1.subspan(1, span1.size() - 2)}; // (2)
    printMe(span2);
    std::transform(span2.begin(), span2.end(), // (3)
        span2.begin(),
        [](int i){ return i * i; }
    );
    printMe(vec);
    printMe(span1);
    printMe(span2);
}