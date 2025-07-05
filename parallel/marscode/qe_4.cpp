// 问题描述
// 小M拿到了一个仅由小写字母组成的字符串，她想知道在这个字符串中，有多少个子序列的首尾字符相同。子序列的定义是：从原字符串中按原顺序取出若干字符（可以不连续）组成的新字符串。

// 例如，对于字符串 "arcaea"，其子序列包括 "aca", "ara", "aaa" 等，这些子序列的首尾字符都是相同的。

// 你需要计算满足这一条件的子序列数量，并输出对 
// 998244353 取模的结果。

// 测试样例
// 样例1：

// 输入：s = "arcaea"
// 输出：28

// 样例2：

// 输入：s = "abcabc"
// 输出：18

// 样例3：

// 输入：s = "aaaaa"
// 输出：31

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

std::unordered_map<int, int> jc_cache;
int jc(int n) {
    if (jc_cache.count(n)) {
        return jc_cache[n];
    }
    if (n == 1) {
        jc_cache[1] = 1;
        return 1;
    }
    int ans = n * jc(n - 1);
    jc_cache[n] = ans;
    return ans;
}

// struct PairHash {
//   template <typename T1, typename T2>
//   std::size_t operator()(const std::pair<T1, T2>& pair) const {
//     auto hash1 = std::hash<T1>{}(pair.first);
//     auto hash2 = std::hash<T2>{}(pair.second);
//     return hash1 * 100 + hash2;
//   }
// };

// std::unordered_map<std::pair<int, int>, int, PairHash> calc_cache;
int calc(int n, int r) {
    if (n == r) {
        return 1;
    }

    // auto k = std::make_pair(n, r);
    // if (calc_cache.count(k)) {
    //     return calc_cache[k];
    // }
    int ans = jc(n - 1) / jc(n - r);
    ans = ans % 998244353;
    // calc_cache[k] = ans;
    return ans;
}

int b_w_c(int idx, int n) {
    int sn = (n - idx);
    if (sn == 1) {
        return 1;
    }
    int ans = 0;
    for (int r = 1; r <= sn; r++) {
        ans += calc(sn, r);
        // ans = ans % 998244353;
    }
    return ans;
}

int solution(std::string s) {
    // PLEASE DO NOT MODIFY THE FUNCTION SIGNATURE
    // write code here
    if (s.empty()) {
        return 0;
    }

    int ans;
    char pch = s[0];
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == pch) {
            ans += b_w_c(i, s.size());
            // ans %= 998244353;
        }
    }
    std::cout << "sese : " << ans  << "\n";
    return ans;
}

int main() {
    std::cout << (solution("arcaea") == 28) << std::endl;
    std::cout << (solution("abcabc") == 18) << std::endl;
    std::cout << (solution("aaaaa") == 31) << std::endl;
    return 0;
}