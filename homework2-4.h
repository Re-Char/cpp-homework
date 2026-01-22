#include "type_registry.h"
#include <map>
#include <string>
#include <any>
#include <iostream>
using namespace std;

struct Day {
    int year_;
    int month_;
    int day_;
    map<string, any> userData;
};

// 添加或更新用户自定义数据
template <typename T>
void setUserData(Day &day, const std::string &key, T value) {
    day.userData[key] = move(value);
}

// 获取用户自定义数据
template <typename T>
T getUserData(Day &day, const std::string &key) {
    if (!day.userData.count(key))
        throw runtime_error("Key not found:" + key);
    return any_cast<T>(day.userData[key]);
}

// 打印基本信息与所有用户数据的类型
void print(Day &day) {
    cout << "Date:" << day.year_ << '-' << day.month_ << '-' << day.day_ << '\n';
    for (auto& [key, data] : day.userData) {
        cout << key << ':';
        TypeRegistry::print(data);
        cout << "\n";
    }
}