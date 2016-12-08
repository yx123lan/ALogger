//
// Created by tarent on 2016/12/8.
//

#include <cwchar>

#ifndef ALOGGER_LOGMANAGER_H
#define ALOGGER_LOGMANAGER_H

#endif //ALOGGER_LOGMANAGER_H
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
    void println(const char *tag, const char *msg);

private:
    const long MEM_ADD = 80 * 1024;

    int logFile;
    int *memData;
    long logFileSize;

    LogManager();
    ~LogManager();
    LogManager(const LogManager&);
    LogManager& operator=(const LogManager&);

    /**
     * 打开日志文件
     */
    int openLogFile(const char *path);
};