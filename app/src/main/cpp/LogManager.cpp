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
        close(logFile);
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
        ssize_t status = write(logFile, &FILE_END, sizeof(FILE_END));
        if (status != -1)
        {
            return OPEN_LOG_FILE_SUCCESS;
        }
    }
    return OPEN_LOG_FILE_FAILURE;
}

void LogManager::expandLogFile()
{
    munmap(memData, (size_t)logFileSize);
    logFileSize = lseek(logFile, logFileOffset, SEEK_END);
    mmapLogFile();
}

/**
 * 初始化日志
 */
void LogManager::init(const char *logFilePath)
{
    ssize_t status = openLogFile(logFilePath);
    if (status == OPEN_LOG_FILE_SUCCESS)
    {
        mmapLogFile();
    }
    else if(status == OPEN_LOG_FILE_FAILURE)
    {
        memData = static_cast<char* > MAP_FAILED;
    }
}

void LogManager::mmapLogFile()
{
    memData = static_cast<char* > (mmap(NULL, MEM_ADD, PROT_WRITE, MAP_SHARED, logFile, logFileSize - MEM_ADD));
    if (logFileSize > (MEM_ADD + sizeof(FILE_END)))
    {
        for (int i = 0; i < MEM_ADD; i++)
        {
            if (*(memData + i) == '\n')
            {
                logFileOffset = i;
            }
        }
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
        const char* line = (time + " [" + tag + "] " + msg + std::string(&LINE_END)).c_str();
        size_t lineByteSize = strlen(line) + 1;
        delete &time;
        if ((logFileOffset + lineByteSize) > MEM_ADD)
        {
            expandLogFile();
        }
        memcpy(memData + logFileOffset, line, lineByteSize);
        logFileOffset += lineByteSize;
    }
}
