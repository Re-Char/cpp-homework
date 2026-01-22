#include <iostream>
#include <string>
#include <vector>
#include <cstddef>
#include <unordered_map>
#include <sstream>
#include <algorithm>
using namespace std;

// 评测默认字段表容量常量
constexpr size_t kDictInitSize = 1024;

class dictEntry {
public:
    string key;
    string value;
    dictEntry *next;
};

class dictht {
public:
    vector<dictEntry *> table;
    size_t size;
    size_t sizemask;
    size_t used;

    void init(size_t init_size_power_of_two) {
        // TODO: 初始化桶数组与掩码，置 used=0；table 元素置为 nullptr
        size = init_size_power_of_two;
        sizemask = size - 1;
        used = 0;
        table.assign(size, nullptr);
    }
    bool set(const string &key, const string &value) {
        // TODO: 计算 idx=hash(key)&sizemask；若命中节点则更新并返回 false，否则头插（新建节点插入到链表头部）并 used++ 返回 true
        size_t idx = std::hash<std::string>{}(key) & sizemask;
        dictEntry *p = table[idx];
        while (p) {
            if (p->key == key) {
                p->value = value;
                return false;
            }
            p = p->next;
        }
        dictEntry *node = new dictEntry();
        node->key = key;
        node->value = value;
        node->next = p;
        table[idx] = node;
        ++used;
        return true;
    }
    const string *get(const string &key) const {
        // TODO: 同桶链表遍历，命中返回值指针；未命中返回 nullptr
        size_t idx = std::hash<std::string>{}(key)&sizemask;
        dictEntry *p = table[idx];
        while (p) {
            if (p->key == key) return &p->value;
            p = p->next;
        }
        return nullptr;
    }
    bool del(const string &key) {
        // TODO: 同桶链表遍历并删除节点，修复链表指针，命中返回 true 并 used--，未命中返回 false
        size_t idx = std::hash<std::string>{}(key)&sizemask;
        dictEntry *prev = nullptr;
        dictEntry *p = table[idx];
        while (p) {
            if (p->key == key) {
                if (prev) prev->next = p->next;
                else table[idx] = p->next;
                --used;
                delete p;
                return true;
            }
            prev = p;
            p = p->next;
        }
        return false;
    }
    size_t len() const {
        // TODO: 返回已存储键值对数量 used
        return used;
    }
    void clear() {
        // TODO: 遍历所有桶释放节点，桶置空，used=0
        for (size_t i = 0; i < size; ++i) {
            dictEntry *cur = table[i];
            while (cur) {
                dictEntry *temp = cur;
                cur = cur->next;
                delete temp;
            }
        }
        table.assign(0, nullptr);
        used = 0;
    }
};

class RedisHash {
public:
    bool hset(const string &key, const string &field, const string &value) {
        // TODO: 若 key 不存在则创建 dictht 并调用 init(kDictInitSize)；随后调用 set(field,value)，返回新增/更新标志
        if (!db.count(key)) {
            dictht temp;
            temp.init(kDictInitSize);
            db[key] = temp;
        }
        return db[key].set(field, value);
    }
    const string *hget(const string &key, const string &field) const {
        // TODO: 若 key 不存在返回 nullptr；存在则在对应 dictht 上执行 get(field)
        if (!db.count(key)) return nullptr;
        return db.at(key).get(field);
    }
    bool hdel(const string &key, const string &field) {
        // TODO: 若 key 不存在返回 false；存在则在对应 dictht 上执行 del(field)
        if (!db.count(key)) return false;
        return db[key].del(field);
    }
    size_t hlen(const string &key) const {
        // TODO: 若 key 不存在返回 0；存在则返回对应 dictht.len()
        if (!db.count(key)) return 0;
        return db.at(key).len();
    }
    void flushall() {
        // TODO: 对所有 dictht 执行 clear() 并清空键空间映射
        for (auto& dic : db) dic.second.clear();
    }

private:
    unordered_map<string, dictht> db;
};
int main() {

    RedisHash store;

    string line;
    while (getline(cin, line)) {
        if (line.empty())
            continue;
        istringstream iss(line);
        vector<string> tok;
        string t;
        while (iss >> t)
            tok.push_back(t);
        if (tok.empty())
            continue;

        const string &cmd = tok[0];
        if (cmd == "HSET") {
            bool added = store.hset(tok[1], tok[2], tok[3]);
            cout << (added ? 1 : 0) << '\n';
        }
        else if (cmd == "HGET") {
            const string *v = store.hget(tok[1], tok[2]);
            cout << (v ? *v : string("NULL")) << '\n';
        }
        else if (cmd == "HDEL") {
            bool ok = store.hdel(tok[1], tok[2]);
            cout << (ok ? 1 : 0) << '\n';
        }
        else if (cmd == "HLEN") {
            size_t cnt = store.hlen(tok[1]);
            cout << cnt << '\n';
        }
        else if (cmd == "FLUSHALL") {
            store.flushall();
            cout << "OK\n";
        }
    }
    return 0;
}