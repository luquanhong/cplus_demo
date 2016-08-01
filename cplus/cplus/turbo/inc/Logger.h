#ifndef __TURBO_LOG_H__
#define __TURBO_LOG_H__

#if defined(__APPLE__)
//  #include <Foundation/Foundation.h> // NSLog is defined in a header file contain objc syntax, so we must compile with -objC for apple platforms
#elif defined(__ANDROID__)
#include <android/log.h> // the log functions defined in NDK
#endif

#include <stdarg.h>

#include "Mutex.h"


#ifdef ENABLE_FILE_LOG
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define LOG_MAX_SIZE 4096
#endif

namespace turbo {

class Logger {

public:
    // NOTE: also change prebuild.py when change this enum
    enum Level {
        LEVEL_VERBOSE = 0,
        LEVEL_DEBUG = 1,
        LEVEL_INFO = 2,
        LEVEL_WARNING = 3,
        LEVEL_ERROR = 4,
        LEVEL_FATAL = 5,
        __LAST__ = LEVEL_FATAL
    }; // enum Level;

public:
    Logger(const char* tag);
    ~Logger();

public:
    static void log(const char* tag, Level level, const char* format, ...);
    static void log(const char* tag, Level level, const char* format, va_list args);

    static void v(const char* tag, const char* format, ...);
    static void d(const char* tag, const char* format, ...);
    static void i(const char* tag, const char* format, ...);
    static void w(const char* tag, const char* format, ...);
    static void e(const char* tag, const char* format, ...);
    static void f(const char* tag, const char* format, ...);
    static void s(const char* tag, const char* format, ...);

    static void setLogLevel(Level level)
    {
        _getDefaultLogger()->setLevel(level);
    }
#ifdef ENABLE_FILE_LOG	
    static void setLogFd(int fd)
    {
        _getDefaultLogger()->setFd(fd);
    }
#endif

public:
    void log(char* tag, Level level, const char* format, ...);
    // This is what platform implementation dependent
    void log(char* tag, Level level, const char* format, va_list args);

    void v(const char* format, ...);
    void d(const char* format, ...);
    void i(const char* format, ...);
    void w(const char* format, ...);
    void e(const char* format, ...);
    void f(const char* format, ...);
    void s(const char* format, ...);

    void setLevel(Level level)
    {
        _level = level;
    }
#ifdef ENABLE_FILE_LOG	
    void setFd(int fd)
    {
        _fd = fd;
    }
#endif
private:
    //StringX & getFormat(const char * format); //add by zzj

private:
    // This default Logger is used for static methods.
    static Logger* _defaultLogger;
    static Mutex _defaultLoggerLock;

    Level _level;
    Mutex _lock;
#ifdef ENABLE_FILE_LOG	
    int _fd;
    char buffer[LOG_MAX_SIZE];
#endif
private:
#if defined(__ANDROID__)
    static android_LogPriority _androidLogPriorities[__LAST__ - LEVEL_VERBOSE + 1];
    static android_LogPriority _getAndroidLogPriority(Level level)
    {
        return _androidLogPriorities[level];
    }
#endif

    // Use one character for each level, as android did.
    static char _logLevelChars[__LAST__ - LEVEL_VERBOSE + 1];
    static char _getLogLevelChar(Level level)
    {
        return _logLevelChars[level];
    }
    static void _logPrefix(const char* tag, Level level);

    static Logger* _getDefaultLogger();

private:
    Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);

}; // class Log

#define LOG_CONVENIENT_METHOD(v, l)                                 \
    inline void Logger::v(const char* tag, const char* format, ...) \
    {                                                               \
        va_list args;                                               \
        va_start(args, format);                                     \
                                                                    \
        log(tag, (l), format, args);                                \
                                                                    \
        va_end(args);                                               \
    }

LOG_CONVENIENT_METHOD(v, Logger::LEVEL_VERBOSE)
LOG_CONVENIENT_METHOD(d, Logger::LEVEL_DEBUG)
LOG_CONVENIENT_METHOD(i, Logger::LEVEL_INFO)
LOG_CONVENIENT_METHOD(w, Logger::LEVEL_WARNING)
LOG_CONVENIENT_METHOD(e, Logger::LEVEL_ERROR)
LOG_CONVENIENT_METHOD(f, Logger::LEVEL_FATAL)
LOG_CONVENIENT_METHOD(s, Logger::LEVEL_FATAL)

} // namespace turbo

#define assertuc(p)                                                                                                 \
    if (!(p)) {                                                                                                     \
        Logger::e("MediaPlayer", "assertuc: failedAssertion failed: %s, file %s, line %d", #p, __FILE__, __LINE__); \
    }

// FIXME: use Logger::v instead of DEBUGLOG
//DebugLog uses for high frequency logs
//must be disabled when deliver to user
#define DEBUGLOG 0 // This line will be changed by buildbot. see prebuild.py
#if DEBUGLOG
#define DebugLog Logger::d
#else
#define DebugLog
#endif // DEBUGLOG

#endif // __TURBO_LOG_H__
