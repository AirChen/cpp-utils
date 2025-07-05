// 问题描述
// 小A拿到了一个长度为
// n
// n的数组，并且定义了一个连续子数组的“权值”为该子数组内不同元素的个数。现在，她想知道，权值分别为
// 1
// ,
// 2
// ,
// 3
// ,
// …
// ,
// n
// 1,2,3,…,n的子数组数量有多少个。

// 你需要根据给定的数组，输出一个包含
// n
// n个整数的数组，第
// i
// i个数表示权值为
// i
// i的子数组数量。

// 测试样例
// 样例1：

// 输入：n = 4, a = [1, 2, 2, 3]
// 输出：[5, 4, 1, 0]

// 样例2：

// 输入：n = 3, a = [1, 1, 1]
// 输出：[6, 0, 0]

// 样例3：

// 输入：n = 5, a = [1, 2, 3, 2, 1]
// 输出：[5, 5, 5, 0, 0]

#include <iostream>
#include <vector>
#include <set>

using namespace std;

vector<int> solution(int n, vector<int>& a) {
    vector<int> c(n + 1, 0);
    vector<vector<set<int>>> dp(n, vector<set<int>>(n));
    for (int i = 0; i < n; i++) {
        dp[i][i].insert(a[i]);
    }
    for (int i = 0; i < n; i++) {
        int l = i - 1, r = i + 1;
        while (l >= 0) {
            dp[l][i] = dp[l + 1][i];
            dp[l][i].insert(a[l]);
            l--;
        }
        while (r < n) {
            dp[i][r] = dp[i][r - 1];
            dp[i][r].insert(a[r]);
            r++;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            c[dp[i][j].size()]++;
        }
    }

    return vector<int>(c.begin() + 1, c.end());
}

int main() {
    vector<int> res;
    vector<int> v1 = {1, 2, 2, 3};
    vector<int> v2 = {1, 1, 1};
    vector<int> v3 = {1, 2, 3, 2, 1};

    res = solution(4, v1);
    cout << (res == vector<int>{5, 4, 1, 0}) << endl;

    res = solution(3, v2);
    cout << (res == vector<int>{6, 0, 0}) << endl;

    res = solution(5, v3);
    cout << (res == vector<int>{5, 5, 5, 0, 0}) << endl;

    return 0;
}