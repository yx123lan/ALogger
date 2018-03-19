//
// Created by tarent on 2018/3/18.
//

#ifndef ALOGGER_LOGGER_H
#define ALOGGER_LOGGER_H


#include <string>

class Logger {
public:

    const int OPEN_FILE_FAILURE = -1;
    const int OPEN_FILE_SUCCESS = 1;

    Logger();
    void println(const char * tag, const char * msg);
    int openLogFile(const char *logFilePath);
    ~Logger();

private:
    int logFile = OPEN_FILE_FAILURE;
    off_t logFileLength = 0;
};


#endif //ALOGGER_LOGGER_H
