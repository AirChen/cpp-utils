#include <tuple>
#include <iostream>

int main() {
    using std::get;
    std::tuple<std::string, int, float> tup1("first", 3, 4.17f);
    auto tup2= std::make_tuple("second", 4, 1.1);
    
    std::cout << get<0>(tup1) << ", " << get<1>(tup1) << ", "
    << get<2>(tup1) << '\n'; // first, 3, 4.17
    std::cout << get<0>(tup2) << ", " << get<1>(tup2) << ", "
    << get<2>(tup2) << '\n'; // second, 4, 1.1
    std::cout << (tup1 < tup2) << '\n'; // true
    
    get<0>(tup2)= "Second";
    std::cout << get<0>(tup2) << "," << get<1>(tup2) << ","
    << get<2>(tup2) << '\n'; // Second, 4, 1.1
    std::cout << (tup1 < tup2) << '\n'; // false
    
    auto pair= std::make_pair(1, true);
    std::tuple<int, bool> tupx = pair;

    int first= 1;
    int second= 2;
    int third= 3;
    int fourth= 4;
    std::cout << first << " " << second << " "
    << third << " " << fourth << std::endl; // 1 2 3 4
    
    auto tup = std::tie(first, second, third, fourth) // bind the tuple
             = std::make_tuple(101, 102, 103, 104); // create the tuple
    
    // and assign it
    std::cout << get<0>(tup) << " " << get<1>(tup) << " " << get<2>(tup)
    << " " << get<3>(tup) << std::endl; // 101 102 103 104
    std::cout << first << " " << second << " " << third << " "
    << fourth << std::endl; // 101 102 103 104
    
    first= 201;
    get<1>(tup)= 202;
    std::cout << get<0>(tup) << " " << get<1>(tup) << " " << get<2>(tup)
    << " " << get<3>(tup) << std::endl; // 201 202 103 104
    std::cout << first << " " << second << " " << third << " "
    << fourth << std::endl; // 201 202 103 104
    
    int a, b;
    std::tie(std::ignore, a, std::ignore, b)= tup;
    std::cout << a << " " << b << std::endl; // 202 104
}
