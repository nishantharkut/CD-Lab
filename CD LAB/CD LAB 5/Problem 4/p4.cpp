#include <bits/stdc++.h>
using namespace std;

bool isMatch(const string &s, const string &p) {
    int m = s.size(), n = p.size();
    vector<vector<char>> dp(m+1, vector<char>(n+1, 0));
    dp[m][n] = 1;
    for (int i = m; i >= 0; --i) {
        for (int j = n-1; j >= 0; --j) {
            bool first = (i < m) && (p[j] == s[i] || p[j] == '.');
            if (j+1 < n && p[j+1] == '*') {
                dp[i][j] = dp[i][j+2] || (first && dp[i+1][j]);
            } else {
                dp[i][j] = first && dp[i+1][j+1];
            }
        }
    }
    return dp[0][0];
}

int main() {
    int t;
    cout << "Enter number of test cases: ";
    cin >> t;
    while (t--) {
        string s, p;
        cout << "Enter string s: ";
        cin >> s;
        cout << "Enter pattern p: ";
        cin >> p;
        cout << "Result: " << (isMatch(s, p) ? "True" : "False") << '\n';
    }
    return 0;
}
