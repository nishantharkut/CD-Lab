#include <bits/stdc++.h>
using namespace std;

int countSubarraysWithXor(vector<int> &arr, int k)
{
    unordered_map<int, int> freq;
    int prefixXor = 0, count = 0;

    for (int num : arr)
    {
        prefixXor ^= num;

        if (prefixXor == k)
            count++;

        int required = prefixXor ^ k;
        if (freq.find(required) != freq.end())
        {
            count += freq[required];
        }

        freq[prefixXor]++;
    }
    return count;
}

int main()
{
    int t;
    cout << "Enter number of test cases: ";
    cin >> t;
    while (t--)
    {
        int n, k;
        cout << "Enter size of array: ";
        cin >> n;

        vector<int> arr(n);
        cout << "Enter array elements: ";
        for (int i = 0; i < n; i++)
        {
            cin >> arr[i];
        }

        cout << "Enter target XOR (k): ";
        cin >> k;

        int result = countSubarraysWithXor(arr, k);
        cout << "Output: " << result << "\n";

        
    }
    return 0;
}
