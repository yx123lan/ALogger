//
// Created by tarent on 2016/12/8.
//
#include "LogManager.h"

#include <errno.h>
#include <sys/uio.h>
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
        close(logFile);
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
    logFileSize = lseek(logFile, 0, SEEK_END);
    // 文件大小是否等于0，是的话说明是刚创建的文件，需要扩充一定的大小
    if(logFileSize == 0)
    {
        // 准备扩充文件大小
        logFileSize = lseek(logFile, MEM_ADD - sizeof(FILE_END), SEEK_END);
        // 判断是否申请扩充成功
        if (logFileSize > 0)
        {
            // 将文件结束符号写到末尾
            ssize_t status = write(logFile, &FILE_END, sizeof(FILE_END));
            // 判断最终是否扩充成功
            if (status != -1)
            {
                logFileSize += sizeof(FILE_END);
                return OPEN_LOG_FILE_SUCCESS;
            }
        }
    } else {
        return OPEN_LOG_FILE_SUCCESS;
    }
    return OPEN_LOG_FILE_FAILURE;
}

void LogManager::expandLogFile()
{
    // 取消映射
    munmap(memData, (size_t)logFileSize);
    // 扩充文件
    logFileSize = lseek(logFile, MEM_ADD - sizeof(FILE_END), SEEK_END);
    if (logFileSize > 0) {
        ssize_t status = write(logFile, &FILE_END, sizeof(FILE_END));
        if (status != -1)
        {
            logFileSize += sizeof(FILE_END);
            logFileOffset = 0;
            // 重新映射
            mmapLogFile();
        }
    }
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
    off_t offset = logFileSize - MEM_ADD;
    memData = static_cast<char* > (mmap(NULL, MEM_ADD, PROT_WRITE | PROT_READ, MAP_SHARED, logFile, offset));
    if (memData == MAP_FAILED)
    {
        return;
    }
    for (int i = 0; i < MEM_ADD; i++)
    {
        if (*(memData + i) == '\n')
        {
            logFileOffset = i;
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
        if (memData != MAP_FAILED)
        {
            memcpy(memData + logFileOffset, line, lineByteSize);
            logFileOffset += lineByteSize;
        }
    }
}
