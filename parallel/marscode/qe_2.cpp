// 问题描述
// 小C有一个由数字字符组成的字符串，她想对这个字符串进行修改，使修改后的字符串中没有连续相同的字符。她需要计算至少进行多少次修改，才能确保字符串中的每两个连续字符不同。

// 测试样例
// 样例1：

// 输入：s = "111222333"
// 输出：3

// 样例2：

// 输入：s = "11551111"
// 输出：4

// 样例3：

// 输入：s = "1234567890"
// 输出：0

#include <iostream>
#include <string>
using namespace std;

int solution(string s) {
    // PLEASE DO NOT MODIFY THE FUNCTION SIGNATURE
    // write code here
    int ans = 0;
    for (int i = 1; i < s.size(); i++) {
        if (s[i] == s[i - 1]) {
            ans++;
            i++;
        }
    }
    return ans;
}

int main() {
    cout << (solution("111222333") == 3) << endl;
    cout << (solution("11551111") == 4) << endl;
    cout << (solution("1234567890") == 0) << endl;
    return 0;
}