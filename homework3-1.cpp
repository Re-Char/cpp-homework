#pragma GCC optimize(2)

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define INF 0x7fffffff

long long minSum = INF;

void dfs(int source, const vector<vector<int>>& distances, vector<int>& nodes,  int depth, long long pathSum) {
    int size = nodes.size();
    if (depth == size) minSum = min(minSum, pathSum);
    for (int i = 0; i < size; ++i) {
        if (nodes[i] < 0 || (i >= size / 2 && nodes[i - size / 2] > 0) || distances[source][nodes[i] - 1] == INF)
            continue;
        int temp = nodes[i] - 1;
        nodes[i] = -nodes[i];
        dfs(temp, distances, nodes, depth + 1, pathSum + distances[source][temp]);
        nodes[i] = -nodes[i];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<int> nodes(2 * n);
    vector<vector<int>> distances(m, vector<int>(m));
    for (int i = 0; i < m; ++i) 
        for (int j = 0; j < m; ++j)
            cin >> distances[i][j];
    for (int i = 0; i < n; ++i) 
        cin >> nodes[i] >> nodes[i + n];
    vector<vector<pair<int, int>>> adjs(m);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            if (distances[i][j] != -1)
                adjs[i].emplace_back(j, distances[i][j]);
    for (int i = 0; i < m; ++i) {
        distances[i].assign(m, INF);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        distances[i][i] = 0;
        pq.emplace(0, i);
        while (!pq.empty()) {
            auto [dis, source] = pq.top();
            pq.pop();
            if (dis != distances[i][source]) continue;
            for (auto& adj : adjs[source]) {
                if (adj.second + dis < distances[i][adj.first]) {
                    distances[i][adj.first] = adj.second + dis;
                    pq.emplace(adj.second + dis, adj.first);
                }
            }
        }
    }
    dfs(0, distances, nodes, 0, 0);
    cout << minSum << "\n";
    return 0;
}