//
// Created by tarent on 2016/12/8.
//
#include "LogManager.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

LogManager::LogManager()
{

}

LogManager::~LogManager()
{
    close(logFile);
    if (memData != MAP_FAILED)
    {
        munmap(memData, logFileSize);
    }
}

LogManager::LogManager(const LogManager &)
{

}

LogManager& LogManager::operator=(const LogManager &)
{

}
/**
 * 打开日志文件
 */
int LogManager::openLogFile(const char *path)
{
    return open(path, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
}

/**
 * 初始化日志
 */
void LogManager::init(const char *logFilePath)
{
    logFile = openLogFile("");
    logFileSize = lseek(logFile, MEM_ADD, SEEK_END);
    memData = static_cast<int*> (mmap(NULL, (size_t)logFileSize, PROT_WRITE, MAP_SHARED, logFileSize - MEM_ADD, 0));
}
/**
 * 输出日志
 */
void LogManager::println(const char* tag, const char* msg)
{
    if (memData != MAP_FAILED) {

    }
}
