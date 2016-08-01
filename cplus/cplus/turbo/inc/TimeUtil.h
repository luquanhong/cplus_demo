// --------------------------------------------------------------------------------------------
// TimeUtil.h
//
// Collect time related utilities temporarily.
//
// --------------------------------------------------------------------------------------------
#ifndef __TURBO_TIME_UTIL_H__
#define __TURBO_TIME_UTIL_H__

#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#define START_MESURE_TIME(s) double t##s = turbo::TimeUtil::getRealTimeS();
#define END_MESURE_TIME(s, e) double t##e = turbo::TimeUtil::getRealTimeS(); Logger::d(TAG, "time elapsed %lld\n", t##e-t##s);

namespace turbo {

class TimeUtil {

public:
    const static int64_t kUsPerSecond = 1000000LL; // 10e6
    const static int64_t kUsPerMs = 1000LL; // 10e3
    const static int64_t kNsPerUs = 1000LL; // 10e3
    const static int64_t kMsPerSecond = 1000LL; // 10e3

    const static int64_t kInvalidTimeUsValue = -1LL; // Mark this is an invalid time value

public:
    static int64_t getRealTimeUs();
    static int64_t getRealTimeMs();
    static double getRealTimeS();
    static void getTimeSpecFromUs(struct timespec* pTS, int64_t us);

}; // class TimeUtil

inline int64_t TimeUtil::getRealTimeUs()
{
    struct timeval tv;
    gettimeofday(&tv, 0);

    return (int64_t)tv.tv_sec * kUsPerSecond + (int64_t)tv.tv_usec;
}
    
inline int64_t TimeUtil::getRealTimeMs()
{
    return getRealTimeUs() / 1000;
}

inline double TimeUtil::getRealTimeS()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    
    return (double)tv.tv_sec + tv.tv_usec/1000000.0;
}

inline void TimeUtil::getTimeSpecFromUs(struct timespec* pTS, int64_t us)
{
    pTS->tv_sec = (time_t)(us / kUsPerSecond);
    pTS->tv_nsec = (long)((us % kUsPerSecond) * kNsPerUs);
}

} // namespace turbo

#endif // __TURBO_TIME_UTIL_H__
