//
// Created by tarent on 2018/3/18.
//

#include "Logger.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

const char * LINE_END = "\n";
const char * TAG_PREFIX = " [";
const char * TAG_SUFFIX = "] ";

Logger::Logger() {

}

int Logger::openLogFile(const char * logFilePath) {
    logFile = open(logFilePath, O_RDWR | O_APPEND | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    logFileLength = lseek(logFile, 0, SEEK_END);
    if (logFile != OPEN_FILE_FAILURE) {
        return OPEN_FILE_SUCCESS;
    } else {
        return OPEN_FILE_FAILURE;
    }
}

char * getCurrentFormatTimeString()
{
    struct timeval  tv;
    char * timeArray = new char[64];
    gettimeofday(&tv, NULL);
    memset(timeArray, 0, sizeof(timeArray));
    strftime(timeArray, sizeof(timeArray) - 1, "%F %T", localtime(&tv.tv_sec));
    char msTimeArray[16];
    sprintf(msTimeArray, "%ld", tv.tv_usec);
    strcat(strcat(timeArray, "."), msTimeArray);
    return timeArray;
}

void Logger::println(const char * tag, const char * msg) {
    if (logFile == OPEN_FILE_FAILURE) {
        return;
    }
    char * time = getCurrentFormatTimeString();
    size_t lineLength = strlen(time) + strlen(TAG_PREFIX) + strlen(tag) + strlen(TAG_SUFFIX) + strlen(msg) + strlen(LINE_END)  + 1;
    char * line = new char[lineLength];
    strcpy(line, time);
    strcat(line, TAG_PREFIX);
    strcat(line, tag);
    strcat(line, TAG_SUFFIX);
    strcat(line, msg);
    strcat(line, LINE_END);
    // 算上字符串末尾的 \0 长度
//    off_t length = lseek(logFile, lineByteSize, SEEK_END);
    int offsetPageCount = logFileLength / getpagesize();
    int offset = offsetPageCount * getpagesize();
    // 溢出无法偏移的部分
    int offsetOverflow = logFileLength % getpagesize();
    int result = ftruncate(logFile, logFileLength + lineLength);
    if (result == 0) {
        char * memData = static_cast<char* > (mmap(NULL, offsetOverflow + lineLength, PROT_WRITE | PROT_READ, MAP_SHARED, logFile, offset));
        if (memData != MAP_FAILED) {
            memcpy(memData + offsetOverflow, line, lineLength);
            logFileLength += lineLength;
        }
        munmap(memData, offsetOverflow + lineLength);
    }
    delete[] line;
    delete[] time;
}

Logger::~Logger() {
    if (logFile != OPEN_FILE_FAILURE) {
        close(logFile);
    }
}