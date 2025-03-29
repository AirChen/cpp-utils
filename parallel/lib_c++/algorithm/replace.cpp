// replace.cpp
#include <algorithm>
#include <iostream>

int main() {
    std::string str{"Only for testing purpose.\n"};
    std::replace(str.begin(), str.end(), ' ', '1');
    std::cout << str; // Only1for1testing1purpose.

    std::replace_if(str.begin(), str.end(), [](char c){ return c == '1'; }, '2');
    std::cout << str; // Only2for2testing2purpose.

    std::string str2;
    std::replace_copy(str.begin(), str.end(), std::back_inserter(str2), '2', '3');
    std::cout << str2; // Only3for3testing3purpose.

    std::string str3;
    std::replace_copy_if(str2.begin(), str2.end(),
    std::back_inserter(str3), [](char c){ return c == '3'; }, '4');
    std::cout << str3; // Only4for4testing4purpose.

    std::cout << str;

    return 0;
}