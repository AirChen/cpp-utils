#include <iostream>
#include <string>
#include <type_traits>

template<typename T = std::string>
void take_out(const T& content) {
    std::cout << "this is " << content << "\n";
}

template<typename T1, typename T2>
T1 orin_max(const T1& a, const T2& b) {
    return b < a ? a : b;
}

template<typename T1, typename T2, typename RT>
RT max(const T1& a, const T2& b) {
    return b < a ? a : b;
}

template<typename RT, typename T1, typename T2>
RT max_v2(const T1& a, const T2& b) {
    return b < a ? a : b;
}

template<typename T1, typename T2>
auto max_v3(const T1& a, const T2& b) -> typename std::decay<decltype(b < a ? a : b)>::type {
    return b < a ? a : b;
}

template<typename T1, typename T2>
std::common_type_t<T1, T2> max_v4(const T1& a, const T2& b) {
    return b < a ? a : b;
}

int main() {
    take_out(12.3);
    take_out("hello world");
    
    // problem 1 return type
    std::cout << ::orin_max(4, 7.2) << "\n";
    std::cout << ::orin_max(7.2, 4) << "\n";
    
    std::cout << ::max<int, double, double>(4, 7.2) << "\n";
    std::cout << ::max<double, int, double>(7.2, 4) << "\n";
    
    std::cout << ::max_v2<double>(4, 7.2) << "\n";
    std::cout << ::max_v2<double>(7.2, 4) << "\n";
    
    std::cout << ::max_v3(4, 7.2) << "\n";
    std::cout << ::max_v3(7.2, 4) << "\n";
    
    std::cout << ::max_v4(4, 7.2) << "\n";
    std::cout << ::max_v4(7.2, 4) << "\n";
    return 0;
}
