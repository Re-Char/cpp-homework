#include <unordered_map>
#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
using namespace std;

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int main() {
    unordered_map<string, int> months = { {"January", 1}, {"February", 2}, {"March", 3}, {"April", 4}, {"May", 5}, {"June", 6}, {"July", 7}, {"August", 8}, {"September", 9}, {"October", 10}, {"November", 11}, {"December", 12} };
    unordered_map<int, int> days = {{1, 31}, {2, 28}, {3, 31}, {4, 30}, {5, 31}, {6, 30}, {7, 31}, {8, 31}, {9, 30}, {10, 31}, {11, 30}, {12, 31}};
    vector<int> startBeiJing = {2024, 9, 16, 0, 0, 0};
    vector<int> startClock = {2024, 9, 1, 22, 20, 0};
    vector<int> input(6, 0);
    for (int i = 0; i < 6; ++i) {
        if (i != 1) cin >> input[i];
        else {
            string s;
            cin >> s;
            input[i] = months[s];
        }
    }
    long long second = input[5];
    second += (input[4] - startClock[4]) * 60;
    second += (input[3] - startClock[3]) * 60 * 60;
    long long day = 0;
    for (int i = startClock[0]; i < input[0]; ++i) {
        day += (365 + isLeapYear(i));
    }
    for (int i = 1; i < startClock[1]; ++i) {
        if (i == 2 && isLeapYear(startClock[0])) day--;
        day -= days[i];
    }
    day -= startClock[2];
    for (int i = 1; i < input[1]; ++i) {
        if (i == 2 && isLeapYear(input[0])) ++day;
        day += days[i];
    }
    day += input[2];
    second += day * 24 * 60 * 60;
    bool flag = second >= 0;
    second = abs(second);
    vector<int> output(6, 0);
    int minute = second / 59;
    second %= 59;
    int hour = minute / 60;
    minute %= 60;
    int totalDays = hour / 24;
    hour %= 24;
    int year = startBeiJing[0];
    int month = startBeiJing[1];
    if (flag) {
        output[5] = second;
        output[4] = minute;
        output[3] = hour;
        totalDays += startBeiJing[2];
        while (totalDays > 0) {
            int monthDays = days[month];
            if (month == 2 && isLeapYear(year))
                ++monthDays;
            if (totalDays <= monthDays) {
                break;
            } else {
                totalDays -= monthDays;
                ++month;
                if (month == 13) {
                    month = 1;
                    ++year;
                }
            }
        }
        output[0] = year;
        output[1] = month;
        output[2] = totalDays;
    } else {
        if (second) {
            startBeiJing[4]--;
            output[5] = 59 - second;
        }
        if (minute > startBeiJing[4]) {
            startBeiJing[3]--;
            output[4] = startBeiJing[4] + 60 - minute;
        }
        if (hour > startBeiJing[3]) {
            startBeiJing[2]--;
            output[3] = 24 + startBeiJing[3] - hour;
        }
        totalDays = -totalDays - startBeiJing[2];
        // cout << totalDays << endl;
        while (totalDays < 0) {
            int monthDays = days[month];
            if (month == 2) ++monthDays;
            if (-totalDays <= monthDays) {
                totalDays += monthDays;
                break;
            } else if (-totalDays > monthDays) {
                totalDays += monthDays;
                month--;
                if (month == 0) {
                    month = 12;
                    year--;
                }
            }
        }
        output[0] = year;
        output[1] = month;
        output[2] = totalDays;
    }
    cout << output[0] << "-"
         << setw(2) << setfill('0') << output[1] << "-"
         << setw(2) << setfill('0') << output[2] << "T"
         << setw(2) << setfill('0') << output[3] << ":"
         << setw(2) << setfill('0') << output[4] << ":"
         << setw(2) << setfill('0') << output[5] << endl;
    return 0;
}