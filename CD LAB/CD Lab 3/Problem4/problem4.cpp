#include <bits/stdc++.h>
using namespace std;

vector<int> asteroid(vector<int> arr){
    if (arr.empty()) return {};
    vector<int> cur = arr;
    bool changed = true;
    while (changed) {
        changed = false;
        vector<int> next;
        int n = (int)cur.size();
        int i = 0;
        while (i < n) {
            if (i + 1 < n && cur[i] > 0 && cur[i+1] < 0) {
                int left = cur[i];
                int right = cur[i+1];
                if (abs(left) > abs(right)) {
                    next.push_back(left);
                } else if (abs(left) < abs(right)) {
                    next.push_back(right);
                } 
                i += 2;
                changed = true;
            } else {
                next.push_back(cur[i]);
                i += 1;
            }
        }
        cur.swap(next);
    }
    return cur;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        vector<int> asteroids(n);
        for (int i = 0; i < n; i++) cin >> asteroids[i];

        vector<int> result = asteroid(asteroids);
        cout << "after destruction, this is the resultant\n";
        cout << "[";
        for (size_t i = 0; i < result.size(); ++i) {
            if (i) cout << ",";
            cout << result[i];
        }
        cout << "]" << '\n';
    }
    return 0;
}
