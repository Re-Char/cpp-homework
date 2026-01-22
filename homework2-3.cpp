#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    double target, k;
    cin >> n >> target >> k;
    vector<int> fund(n);
    for (int i = 0; i < n; ++i)
        cin >> fund[i];
    double temp = 0;
    for (int price : fund)
        temp += (double)price / target;
    int maxPrice = temp + k;
    // cout << maxPrice << "\n";
    bool flag = false;
    int res = 0;
    for (int i = maxPrice; i > 0; --i) {
        double buckets = 0;
        int reserveFund = 0;
        for (int price : fund) {
            buckets += price / i;
            reserveFund += price % i;
        }
        buckets += (double)reserveFund / (double)(i - k);
        if (buckets >= target) {
            flag = true;
            res = i;
            break;
        }
    }
    if (flag) cout << res << "\n";
    else cout << -1 << "\n";
    return 0;
}