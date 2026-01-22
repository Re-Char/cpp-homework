#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<vector<int>> clubs(n, vector<int>(2));
    for (int i = 0; i < n; ++i) {
        cin >> clubs[i][0] >> clubs[i][1];
    }
    sort(clubs.begin(), clubs.end(), [](auto &a, auto &b)
        { return a[1] < b[1]; });
    int time = 0;
    int distance = 0;
    int i = 0;
    while (i < n) {
        int start = clubs[i][0];
        int end = clubs[i][1];
        if (distance >= start) {
            ++i;
            continue;
        }
        if (end <= distance + 27){
            ++i;
            distance = end;
        } else {
            distance += 27;
        }
        ++time;
    }
    cout << time << 'T';
    return 0;
}