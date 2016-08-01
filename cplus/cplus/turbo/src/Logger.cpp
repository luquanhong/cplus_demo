// ----------------------------------------------------------------------------------------
// Logger.cpp
//
// ----------------------------------------------------------------------------------------
#include "Logger.h"
#include "APIVersion.h" // FIXME: should not include in a general purpose lib.

#if defined(__APPLE__)
#include <Foundation/Foundation.h>
#endif

//extern const char* U3PLAYER_VERSION;

namespace turbo {

// Static variables
Logger* Logger::_defaultLogger = 0;
Mutex Logger::_defaultLoggerLock;

#if defined(__ANDROID__)
// static
android_LogPriority Logger::_androidLogPriorities[__LAST__ - LEVEL_VERBOSE + 1] = { ANDROID_LOG_VERBOSE, ANDROID_LOG_DEBUG, ANDROID_LOG_INFO, ANDROID_LOG_WARN, ANDROID_LOG_ERROR, ANDROID_LOG_FATAL };

#endif

// static
char Logger::_logLevelChars[__LAST__ - LEVEL_VERBOSE + 1] = { 'V', 'D', 'I', 'W', 'E', 'F' };

// next line used by buildbot script.
#define DEFAULT_LOG_LEVEL LEVEL_DEBUG
const char* U3PLAYER_ID = "u3playerid:2.0.0.117,trial."; // do not modify, used by buildbot

Logger::Logger()
    : _level(DEFAULT_LOG_LEVEL)
#ifdef  ENABLE_FILE_LOG
    ,_fd(-1)
#endif
{
#ifdef  ENABLE_FILE_LOG
    memset(buffer, 0, LOG_MAX_SIZE);
#endif
}

Logger::~Logger()
{
}

// static
Logger* Logger::_getDefaultLogger()
{
    Mutex::AutoLock lock(_defaultLoggerLock);

    if (_defaultLogger == 0)
        _defaultLogger = new Logger();

    return _defaultLogger;
}

void Logger::log(char* tag, Logger::Level level, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    log(tag, level, format, args);

    va_end(args);
}

// static
void Logger::log(const char* tag, Logger::Level level, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    log(tag, level, format, args);

    va_end(args);
}

// static
void Logger::log(const char* tag, Logger::Level level, const char* format, va_list args)
{
    Logger* pLogger = _getDefaultLogger();

    static int first = 0;
    if (first == 0) {
        first = 1;
        Logger::d("apollo", "%s", U3PLAYER_ID);
    }

#ifdef __ANDROID__
    char* pTag = new char[strlen(tag) + 50];

    char s[64] = { 0 };
    time_t now = time(NULL);
    struct tm tim = *(localtime(&now));
    strftime(s, 64, "%b %d, %Y; %H:%M:%S", &tim);

    sprintf(pTag, "[apollo %s] %s", U3PLAYER_VERSION, tag);
#else // ios
    char* pTag = 0;
#endif // __ANDROID__
    pLogger->log(pTag, level, format, args);
#ifdef __ANDROID__
    delete[] pTag;
#endif // __ANDROID__
}

#if defined(__APPLE__) // ------------------------------------------------------------------

void Logger::log(char* tag, Logger::Level level, const char* format, va_list args)
{
    if (level >= _level) {
#if 0
        NSString* composedFormat = [[NSString alloc] initWithCString:format
                                                            encoding:NSUTF8StringEncoding];
        NSLogv(composedFormat, args);
        [composedFormat release];
#else
        vprintf(format, args);
#endif // 0
    }
}

#elif defined(__ANDROID__) // ------------------------------------------------------------

void Logger::log(char* tag, Logger::Level level, const char* format, va_list args)
{
    if (level >= _level) {
        __android_log_vprint(_getAndroidLogPriority(level), tag, format, args);

#ifdef ENABLE_FILE_LOG
    vsprintf(buffer, format, args);
    if(_fd > 0) {
        write(_fd, buffer, (strlen(buffer) > LOG_MAX_SIZE)? LOG_MAX_SIZE:strlen(buffer));
    }
#endif

    }
}

#else // General --------------------------------------------------------------------------------

#include <time.h>
#include <unistd.h>
#include <pthread.h>

void Logger::_logPrefix(const char* tag, Logger::Level level)
{
    // Prefix
    time_t t;
    time(&t);

    struct tm* tm = localtime(&t);

    fprintf(stderr, "%02d-%02d-%02d %02d:%02d:%02d [%c/%s/%d:%d] ", 1900 + tm->tm_year, tm->tm_mon, tm->tm_day, tm->tm_hour, tm->tm_min, tm->tm_sec,
            _getLogLevelChar(level), tag, getpid(), pthread_self());
}

void Logger::log(char* tag, Logger::Level level, const char* format, va_list args)
{
    if (level >= _level) {
        _logPrefix(tag, level);

        // Contents
        vfprintf(stderr, format, args);
    }
}

#endif

} // namespace turbo
