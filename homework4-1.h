#include <cstring>
#include <iostream>

// 数据类 (无需修改)
class StringData {
private:
    static int next_id;

public:
    // 构造函数：分配内存并初始化
    explicit StringData(const char *str) : id(next_id++) {
        std::strncpy(buffer, str, 1023);
        buffer[1023] = '\0';
        std::cout << "create data " << id << " (" << buffer << ")" << std::endl;
    }

    // 析构函数
    ~StringData() {
        std::cout << "destroy data " << id << std::endl;
    }

    // 辅助构造：用于从现有数据克隆（深拷贝）
    StringData(const StringData &other) : id(next_id++) {
        std::strncpy(buffer, other.buffer, 1024);
        std::cout << "create data " << id << " (copy from " << other.id << ")" << std::endl;
    }

    char buffer[1024];
    int id;
};

// 待实现的 CowString 类
class CowString {
public:
    CowString(const char *str);
    ~CowString();
    CowString(const CowString &other);
    char read(int index) const;
    void write(int index, char c);

    int get_count() const {
        return *_ref_count;
    };

private:
    StringData *_data{nullptr};
    int *_ref_count{nullptr};
};