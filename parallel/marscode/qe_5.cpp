// 问题描述
// 小C有
// n
// n串长度为
// m
// m的糖葫芦，每串糖葫芦可以用一个字符串表示。每个糖葫芦的甜度是所有字符甜度的总和，其中每个字符的甜度为该字符与 'a' 的差值。例如，字符 'a' 的甜度为 
// 0
// 0，字符 'b' 的甜度为 
// 1
// 1，依次类推，字符 'z' 的甜度为 
// 25
// 25。

// 你需要帮助小C找到甜度最大的独一无二的糖葫芦。糖葫芦独一无二当且仅当它与其他
// n
// −
// 1
// n−1根糖葫芦都不同，且翻转后的字符串也不能与其他糖葫芦相同。例如，糖葫芦 "abc" 与 "cba" 视为相同的糖葫芦。

// 如果没有独一无二的糖葫芦，则返回0。

// 测试样例
// 样例1：

// 输入：n = 3, m = 3, strings = ["ccz", "cba", "zcc"]
// 输出：3

// 样例2：

// 输入：n = 2, m = 3, strings = ["abc", "cba"]
// 输出：0

// 样例3：

// 输入：n = 5, m = 2, strings = ["aa", "bb", "ab", "ba", "cc"]
// 输出：4

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int sweetness(const string& s) {
    int sum = 0;
    for (char c : s) {
        sum += c - 'a';
    }
    return sum;
}

int solution(int n, int m, vector<string>& strings) {
    // PLEASE DO NOT MODIFY THE FUNCTION SIGNATURE
    // write code here
    std::map<int, int> sweetness_map;
    for (const auto& s : strings) {
        int sw = sweetness(s);
        sweetness_map[sw]++;
    }

    while (!sweetness_map.empty()) {
        auto it = sweetness_map.rbegin();
        if (it->second == 1) {
            return it->first;
        }
        sweetness_map.erase(it->first);
    }
    return 0;
}

int main() {
    vector<string> strings1 = {"ccz", "cba", "zcc"};
    cout << (solution(3, 3, strings1) == 3) << endl;

    vector<string> strings2 = {"abc", "cba"};
    cout << (solution(2, 3, strings2) == 0) << endl;

    vector<string> strings3 = {"aa", "bb", "ab", "ba", "cc"};
    cout << (solution(5, 2, strings3) == 4) << endl;
}