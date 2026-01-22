#pragma GCC optimize(2)

#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> buys(n);
    // vector<vector<int>> maxProfit(n);
    long long maxSum = 0;
    for (int i = 0; i < m; ++i) {
        int id, price;
        string op;
        cin >> op >> id >> price;
        --id;
        if (op == "buy") {
            buys[id].push_back(price);
            // maxProfit[id].push_back(0);
        }
        for (int j = 0; j < buys[id].size(); ++j) {
            // maxProfit[id][j] = max(price - buys[id][j], maxProfit[id][j]);
            if (price > buys[id][j]) {
                maxSum += price - buys[id][j];
                buys[id][j] = price;
            }
        }
    }
    cout << maxSum << "\n";
    return 0;
}