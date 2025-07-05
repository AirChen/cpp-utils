在C++20标准中，基于范围的for循环增加了对初始化语句的支持
void printVec(const std::vector<int>& vec)
{
    std::cout << "v = {";
    for (char sep[]{0, ' ', 0}; const int i : vec)
        std::cout << sep << i, sep[0] = ',';
    std::cout << "};\n";
}