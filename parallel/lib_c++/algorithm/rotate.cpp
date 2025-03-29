// rotate.cpp
#include <algorithm>
#include <iostream>

int main() {
    std::string str{"12345"};
    for (auto i= 0; i < str.size(); ++i){
        std::string tmp{str};
        std::rotate(tmp.begin(), tmp.begin()+i , tmp.end());
        std::cout << tmp << " ";
    } // 12345 23451 34512 45123 51234
    std::cout << std::endl;

    return 0;
}