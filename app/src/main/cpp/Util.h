//
// Created by tarent on 2016/12/9.
//

#ifndef ALOGGER_UTIL_H
#define ALOGGER_UTIL_H
class Util
{
public:
    static template <class T> int getArrayLen(T& array) {
        return (sizeof(array) / sizeof(array[0]));
    }
};
#endif //ALOGGER_UTIL_H