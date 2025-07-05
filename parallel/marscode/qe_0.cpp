// 问题描述
// 小M在研究字符串时发现了一个有趣的现象：某些字符串是由一个较短的子串反复拼接而成的。如果能够找到这个最短的子串，便可以很好地还原字符串的结构。你的任务是给定一个字符串，判断它是否是由某个子串反复拼接而成的。如果是，输出该最短的子串；否则，输出空字符串""。

// 例如：当输入字符串为 abababab 时，它可以由子串 ab 反复拼接而成，因此输出 ab；而如果输入 ab，则该字符串不能通过子串的重复拼接得到，因此输出空字符串。

// 测试样例
// 样例1：

// 输入：inp = "abcabcabcabc"
// 输出：'abc'

// 样例2：

// 输入：inp = "aaa"
// 输出：'a'

// 样例3：

// 输入：inp = "abababab"
// 输出：'ab'

// 样例4：

// 输入：inp = "ab"
// 输出：''

// 样例5：

// 输入：inp = "abcdabcdabcdabcd"
// 输出：'abcd'

// 样例6：

// 输入：inp = "b"
// 输出：''

#include <iostream>
#include <string>
#include <vector>

std::string solution(const std::string &inp) {
    // Edit your code here
    std::vector<char> mc;
    int b = 0, e = 0;
    int cur = 0;
    bool has_matched = false;
    for (int i = 0; i < inp.size(); ++i) {
        const char& ch = inp[i];
        if (mc.empty()) {
            b = i;
            e = i;
            mc.push_back(ch);
            continue;
        }
        if (b == e) {
            if (ch != mc[0]) {
                mc.push_back(ch);
                e = i;
                continue;
            }
        }

        if (ch == mc[cur]) {
            ++cur;
        } else if (cur == 0) {
            mc.push_back(ch);
            e = i;
        } else {
            for (int j = i - cur; j < i; ++j) {
                mc.push_back(inp[j]);
            }
            i--;
            e = i;
            cur = 0;
        }
        if (cur > e) {
            cur = 0;
            has_matched = true;
        }
    }
    if (has_matched && cur == 0) {
        // std::cout << std::string(mc.begin(), mc.end()) << "\n";
        return std::string(mc.begin(), mc.end());
    }
    return "";
}

int main() {
    // Add your test cases here
    std::cout << (solution("abcabcabcabc") == "abc") << std::endl;
    std::cout << (solution("aaa") == "a") << std::endl;
    std::cout << (solution("abababab") == "ab") << std::endl;
    std::cout << (solution("ab") == "") << std::endl;
    std::cout << (solution("abcdabcdabcdabcd") == "abcd") << std::endl;
    std::cout << (solution("b") == "") << std::endl;
    std::cout << (solution("abaabaabaaba") == "aba") << std::endl;
    std::cout << (solution("abacabacabacaba") == "") << std::endl;
    std::cout << (solution("aababaaab") == "") << std::endl;
    return 0;
}

// def compute_next(pattern):
//     n = len(pattern)
//     next_arr = [0] * n
//     j = 0  # 前缀末尾指针

//     for i in range(1, n):
//         while j > 0 and pattern[i] != pattern[j]:
//             j = next_arr[j - 1]  # 回退到前一个匹配位置
//         if pattern[i] == pattern[j]:
//             j += 1
//         next_arr[i] = j
//     return next_arr

// def kmp_search(text, pattern):
//     next_arr = compute_next(pattern)
//     j = 0  # 模式串指针

//     for i in range(len(text)):
//         while j > 0 and text[i] != pattern[j]:
//             j = next_arr[j - 1]  # 回退
//         if text[i] == pattern[j]:
//             j += 1
//         if j == len(pattern):  # 完全匹配
//             return i - j + 1  # 返回匹配起始位置
//     return -1  # 未匹配
