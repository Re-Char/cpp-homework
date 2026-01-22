#include <cstring>
#include <iostream>
#include <cstdio>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
// using namespace std;

// 日志级别枚举
enum class LogLevel {
    Error = 2,
    Warn = 1,
    Info = 0
};

class LogFile {
private:
    static const int MAX_ENTRIES = 100; // 静态成员：最大日志条数
    static LogLevel systemLevel;        // 静态成员：系统日志级别

    std::string *logEntry_content; // 成员：日志条目内容
    LogLevel *logEntry_level;      // 成员：日志条目级别

    int entryCount;       // 成员：当前已经有的日志条目数量
    char *filename;       // 成员：目标文件
    const int logGroupId; // const成员：日志组名

public:
    // 默认构造函数
    LogFile();

    // 基础构造函数
    LogFile(char *name, const int logGroupId)
        : logEntry_content(new std::string[MAX_ENTRIES]),
          logEntry_level(new LogLevel[MAX_ENTRIES]),
          entryCount(0),
            filename(name ? new char[std::strlen(name) + 1] : nullptr),
        //   filename(name),
          logGroupId(logGroupId) {
        if (filename) std::strcpy(filename, name);
    };

    // 拷贝构造函数
    LogFile(const LogFile &other)
        : logEntry_content(other.logEntry_content ? new std::string[MAX_ENTRIES] : nullptr),
          logEntry_level(other.logEntry_level ? new LogLevel[MAX_ENTRIES] : nullptr),
          entryCount(other.entryCount),
          filename(other.filename ? new char[std::strlen(other.filename) + 1] : nullptr),
          logGroupId(other.logGroupId) {
        if (other.logEntry_content && logEntry_content) {
            for (int i = 0; i < entryCount; ++i) {
                logEntry_content[i] = other.logEntry_content[i];
                logEntry_level[i] = other.logEntry_level[i];
            }
        }
        if (other.filename && filename)
            std::strcpy(filename, other.filename);
    };

    // 移动构造函数
    LogFile(LogFile &&other) noexcept 
    : logEntry_content(other.logEntry_content),
        logEntry_level(other.logEntry_level),
        entryCount(other.entryCount),
        filename(std::move(other.filename)),
        logGroupId(other.logGroupId) {
        other.logEntry_content = nullptr;
        other.logEntry_level = nullptr;
        other.entryCount = 0;
        other.filename = nullptr;
    };

    // 析构函数
    ~LogFile() {
        if (entryCount > 0 && logEntry_level && logEntry_content) {
            // calculate statistics
            int Info_count = 0, Warn_count = 0, Error_count = 0;
            for (int i = 0; i < entryCount; ++i) {
                if (logEntry_level[i] == LogLevel::Error)
                    ++Error_count;
                else if (logEntry_level[i] == LogLevel::Warn)
                    ++Warn_count;
                else if (logEntry_level[i] == LogLevel::Info)
                    ++Info_count;
            }
            outputBeforeCloseFile(Info_count, Warn_count, Error_count);
        }
        // Release memory resources
        delete[] logEntry_content;
        delete[] logEntry_level;
        if (filename) delete[] filename;
    }

    // 添加日志条目（根据系统级别过滤）
    void addEntry(const std::string &content, LogLevel level) {
        // TODO
        if (level < systemLevel) return;
        if (entryCount < MAX_ENTRIES) {
            logEntry_content[entryCount] = content;
            logEntry_level[entryCount++] = level;
        }
    };
    // 寻找日志条目（根据系统级别过滤）
    void findEntry(int index) {
        if (index < 1) return;
        int count = 0;
        for (int i = 0; i < entryCount; ++i) {
            if (systemLevel <= logEntry_level[i]) ++count;
            if (count == index) {
                outputEntry(logEntry_content[i]);
                break;
            }
        }
    }

    static void outputEntry(const std::string &content) {
        std::cout << content << std::endl;
    }
    // 推荐使用该函数输出对应的日志内容

    void outputBeforeCloseFile(int Info_count, int Warn_count, int Error_count) {
        std::cout << "LogGroup " << this->logGroupId << ": File "
                  << this->filename << " saved with "
                  << Error_count << " Error(s), "
                  << Warn_count << " Warn(s), "
                  << Info_count << " Info(s)" << std::endl;
    }
    // 推荐使用该函数输出对应的日志统计信息

    // 静态成员函数：设置系统日志级别
    static void setSystemLevel(LogLevel level) {
        LogFile::systemLevel = level;
    };

    // 静态成员函数：获取系统日志级别
    static LogLevel getSystemLevel() {
        return LogFile::systemLevel;
    };

    int getEntryCount() const {
        return entryCount;
    };

    char *getFilename() const {
        return filename;
    };

    int getLogGroup() const {
        return logGroupId;
    }

    void none_output_before_close_file() {
        this->entryCount = 0;
        delete[] logEntry_content;
        delete[] logEntry_level;
        this->logEntry_content = nullptr;
        this->logEntry_level = nullptr;
    }
};

LogLevel LogFile::systemLevel = LogLevel::Info; // 一开始的日志级别为Info（0）

int main() {

    std::map<std::string, LogFile *> logFiles;

    std::string command;
    LogLevel log_level;
    std::string log_name, file_name, source_name, dest_name, log_context;
    int level, groupId, index;

    while (std::cin >> command) {
        if (command == "create") {
            std::cin >> log_name >> groupId >> file_name;
            logFiles[log_name] = new LogFile(file_name.data(), groupId);
        }

        if (command == "setLevel") {
            std::cin >> level;
            log_level = static_cast<LogLevel>(level);
            LogFile::setSystemLevel(log_level);
        }

        if (command == "add") {
            std::cin >> log_name >> level >> log_context;
            log_level = static_cast<LogLevel>(level);
            if (logFiles.count(log_name))
                logFiles[log_name]->addEntry(log_context, log_level);
        }

        if (command == "find") {
            std::cin >> log_name >> index;

            if (logFiles.count(log_name)) {
                logFiles[log_name]->findEntry(index);
            }
        }

        if (command == "move") {
            std::cin >> source_name >> dest_name;
            logFiles[dest_name] = new LogFile(std::move(*logFiles[source_name]));
            delete logFiles[source_name];
            logFiles.erase(source_name);
        }

        if (command == "copy") {
            std::cin >> source_name >> dest_name;
            if (logFiles.count(source_name)) {
                logFiles[dest_name] = new LogFile(*logFiles[source_name]);
            }
        }

        if (command == "close") {
            std::cin >> log_name;
            if (logFiles.count(log_name)) {
                delete logFiles[log_name];
                logFiles.erase(log_name);
            }
        }

        if (command == "end") {
            break;
        }
    }

    for (auto const &[key, val] : logFiles) {
        val->none_output_before_close_file();
        delete val;
    }
}