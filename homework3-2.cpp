#pragma GCC optimize(2)

#include <string>
#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string cmd;
    getline(cin, cmd);
    vector<int> stack;
    unordered_map<int, int> operateStream;
    for (int i = 0; i < cmd.length(); ++i) {
        if (cmd[i] == '[') stack.push_back(i);
        else if (cmd[i] == ']') {
            int front = stack.back();
            stack.pop_back();
            operateStream.emplace(front, i);
            operateStream.emplace(i, front);
        }
    }
    uint8_t array[1001] = {0};
    int i = 0, pointer = 0;
    while (i < cmd.length()) {
        switch (cmd[i]) {
        case '+':
            ++array[pointer];
            break;
        case '-':
            --array[pointer];
            break;
        case '>':
            ++pointer;
            break;
        case '<':
            --pointer;
            break;
        case '[':
            if (!array[pointer]) i = operateStream[i];
            break;
        case ']':
            if (array[pointer]) i = operateStream[i];
            break;
        case '.':
            cout << static_cast<char>(static_cast<unsigned char>(array[pointer]));
            break;
        case ',':
            char c;
            if (cin.get(c)) array[pointer] = static_cast<uint8_t>(static_cast<unsigned char>(c));
            break;
        default:
            break;
        }
        ++i;
    }
    return 0;
}