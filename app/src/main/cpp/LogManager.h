//
// Created by tarent on 2016/12/8.
//

#include <cwchar>
#include <string>

#ifndef ALOGGER_LOGMANAGER_H
#define ALOGGER_LOGMANAGER_H
class LogManager
{
public:
    static LogManager& getInstance()
    {
        static LogManager instance;
        return instance;
    }
    /**
     * 初始化日志
     */
    void init(const char *logFilePath);

    /**
     * 输出日志
     */
    void println(std::string tag, std::string msg);

private:
    const size_t MEM_ADD = 80 * 1024;
    const char FILE_END = EOF;
    const char LINE_END = '\n';
    const ssize_t OPEN_LOG_FILE_FAILURE = -1;
    const ssize_t OPEN_LOG_FILE_SUCCESS = 1;
    const ssize_t OPEN_LOG_FILE_ALREADY = 2;

    int logFile;
    char *memData;
    long logFileOffset = 0;
    long logFileSize = 0;

    LogManager();
    ~LogManager();
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    /**
     * 初始化日志文件
     */
    int openLogFile(const char *logFilePath);
    void expandLogFile();
    void mmapLogFile();
};
#endif //ALOGGER_LOGMANAGER_H