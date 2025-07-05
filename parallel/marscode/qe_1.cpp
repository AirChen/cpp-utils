// 问题描述
// 小R手中有一个由小写英文字母组成的字符串。她希望将这个字符串转换为回文字符串，并且要求字典序尽可能小。在这个过程中，小R最多可以更改字符串中的两个字符。每个字符可以被更改为任意的小写字母。现在你的任务是帮助小R构造出在满足条件的前提下字典序最小的回文字符串，测试数据保证答案存在。

// 例如：对于字符串 acca，通过更改两个字符，可以得到回文字符串 aaaa，这是字典序最小的解。

// 测试样例
// 样例1：

// 输入：s = "acca"
// 输出：'aaaa'

// 样例2：

// 输入：s = "racecar"
// 输出：'aacecaa'

// 样例3：

// 输入：s = "fecdef"
// 输出：'feaaef'

#include <iostream>
#include <iterator>
#include <vector>
#include <string>

std::string solution(const std::string& s) {
    // PLEASE DO NOT MODIFY THE FUNCTION SIGNATURE
    // write code here
    std::string ans = s;
    int n = s.size();
    bool is_loop = (n != 1);
    bool has_other = false;
    for (int i = 0; i < n / 2; ++i) {
        if (ans[i] != ans[n - 1 - i]) {
            is_loop = false;
            int c = 2;
            if (ans[i] != 'a') {
                ans[i] = 'a';
                c--;
            }
            if (ans[n - i - 1] != 'a') {
                ans[n - i - 1] = 'a';
                c--;
            }
            has_other = (c == 0);
            break;
        }
    }
    if (is_loop) {
        for (int i = 0; i < n / 2; ++i) {
            if (ans[i]!= 'a') {
                ans[i] = 'a';
                ans[n - 1 - i] = 'a';
                break;
            }
        }
    } else if (!has_other) {
        ans[n / 2] = 'a';
    }
    // std::cout << ans << "\n";
    return ans;
}

int main() {
    std::cout << (solution("acca") == "aaaa") << std::endl;
    std::cout << (solution("racecar") == "aacecaa") << std::endl;
    std::cout << (solution("fecdef") == "feaaef") << std::endl;
    std::cout << (solution("acccaacaacccc") == "acccaaaaaccca") << std::endl;
    std::cout << (solution("v") == "a") << std::endl;
    std::cout << (solution("ckb") == "aka") << std::endl;
    return 0;
}