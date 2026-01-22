#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

void handleAdd(string& s, vector<int>& bucket) {
    for (int i = 4; i < s.length(); ++i) {
        ++bucket[s[i] - 'a'];
    }
}

void handleQuery(vector<int>& bucket) {
    int max = bucket[0];
    string res = "";
    for (int i = 1; i < bucket.size(); ++i) {
        if (max < bucket[i])
            max = bucket[i];
    }
    for (int i = 0; i < bucket.size(); ++i) {
        if (max == bucket[i] && bucket[i] != 0)
            res.push_back(i + 'a');
    }
    cout << res << endl;
}

int main() {
    int k = 0;
    cin >> k;
    cin.ignore(1, '\n');
    vector<string> input(k, "");
    vector<int> bucket(26, 0);
    for (int i = 0; i < k; ++i) {
        getline(cin, input[i]);
    }
    for (int i = 0; i < k; ++i) {
        if (input[i][0] == 'a') handleAdd(input[i], bucket);
        else handleQuery(bucket);
    }
    return 0;
}