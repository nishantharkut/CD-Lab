#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, start, end;
};

int minTime(int n, vector<array<int, 4>>& edges) {
    vector<vector<Edge>> graph(n);
    for (auto& e : edges) {
        graph[e[0]].push_back({e[1], e[2], e[3]});
    }

    queue<pair<int, int>> q;
    unordered_set<long long> visited; 
    auto encode = [&](int node, int t) -> long long {
        return ((long long)node << 32) | (long long)t;
    };

    q.push({0, 0});
    visited.insert(encode(0, 0));

    while (!q.empty()) {
        auto [node, t] = q.front();
        q.pop();

        if (node == n - 1) return t;

        if (!visited.count(encode(node, t + 1))) {
            visited.insert(encode(node, t + 1));
            q.push({node, t + 1});
        }

        for (auto& e : graph[node]) {
            if (e.start <= t && t <= e.end) {
                if (!visited.count(encode(e.to, t + 1))) {
                    visited.insert(encode(e.to, t + 1));
                    q.push({e.to, t + 1});
                }
            }
        }
    }

    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 4>> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2] >> edges[i][3];
    }

    int ans = minTime(n, edges);
    cout << "Minimum time to reach node " << n - 1 << ": ";
    cout << ans << "\n";

    return 0;
}
