// 问题描述
// 小U拿到了一个通过特殊方式压缩的字符串，其中每个字母后面可能跟着一个数字，这个数字表示该字母在解压后的字符串中需要重复的次数。如果一个字母后面没有跟随数字，那么该字母在解压后的字符串中只出现一次。请帮助小U解压这个字符串并输出最终的解压结果。

// 测试样例
// 样例1：

// 输入：s = "a2b3c4"
// 输出：'aabbbcccc'

// 样例2：

// 输入：s = "d5ef2"
// 输出：'dddddeff'

// 样例3：

// 输入：s = "x3y1z"
// 输出：'xxxyz'

#include <iostream>
#include <vector>
#include <string>

using namespace std;

string solution(string s) {
    // PLEASE DO NOT MODIFY THE FUNCTION SIGNATURE
    // write code here
    string ans;
    for (int i = 0; i < s.size(); i++) {
        if (i + 1 < s.size() && isdigit(s[i + 1])) {
            int cnt = s[i + 1] - '0';
            ans.append(cnt, s[i]);
            i++;
        } else {
            ans.append(1, s[i]);
        }
    }
    // std::cout << ans << std::endl;
    return ans;
}

int main() {
    cout << (solution("a2b3c4") == "aabbbcccc") << endl;
    cout << (solution("d5ef2") == "dddddeff") << endl;
    cout << (solution("x3y1z") == "xxxyz") << endl;
    return 0;
}