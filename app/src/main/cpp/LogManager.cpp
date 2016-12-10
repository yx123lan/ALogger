//
// Created by tarent on 2016/12/8.
//
#include "LogManager.h"

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

LogManager::LogManager()
{

}

LogManager::~LogManager()
{
    if (memData != MAP_FAILED)
    {
        munmap(memData, (size_t)logFileSize);
    }
}

/**
 * 初始化日志文件
 */
int LogManager::openLogFile(const char *logFilePath)
{
    if (logFile != 0)
    {
        return OPEN_LOG_FILE_ALREADY;
    }
    logFile = open(logFilePath, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    logFileSize = lseek(logFile, MEM_ADD, SEEK_END);
    if (logFileSize > 0)
    {
        char end = EOF;
        ssize_t status = write(logFile, &end, sizeof(end));
        if (status != -1)
        {
            close(logFile);
            return OPEN_LOG_FILE_SUCCESS;
        }
    }
    close(logFile);
    return OPEN_LOG_FILE_FAILURE;
}

/**
 * 初始化日志
 */
void LogManager::init(const char *logFilePath)
{
    ssize_t status = openLogFile(logFilePath);
    if (status == OPEN_LOG_FILE_SUCCESS)
    {
        memData = static_cast<char* > (mmap(NULL, (size_t)logFileSize, PROT_WRITE, MAP_SHARED, logFile, 0));
    }
    else if(status == OPEN_LOG_FILE_FAILURE)
    {
        memData = static_cast<char* > MAP_FAILED;
    }
}

std::string& getCurrentFormatTimeString()
{
    struct timeval  tv;
    char timeArray[64];
    gettimeofday(&tv, NULL);
    memset(timeArray, 0, sizeof(timeArray));
    strftime(timeArray, sizeof(timeArray) - 1, "%F %T", localtime(&tv.tv_sec));
    char msTimeArray[16];
    sprintf(msTimeArray, "%ld", tv.tv_usec);
    strcat(strcat(timeArray, "."), msTimeArray);
    return *(new std::string(timeArray));
}

/**
 * 输出日志
 */
void LogManager::println(std::string tag, std::string msg)
{
    if (memData != MAP_FAILED && msg.length() > 0)
    {
        std::string& time = getCurrentFormatTimeString();
        const char* line = (time + " [" + tag + "] " + msg + "\n").c_str();
        size_t lineByteSize = strlen(line) + 1;
        delete &time;
        memcpy(memData + logFileOffset, line, lineByteSize);
        logFileOffset += lineByteSize;
    }
}
