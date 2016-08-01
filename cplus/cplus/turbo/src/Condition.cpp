// ---------------------------------------------------------------------------------
// Condition.cpp
//
// Holly Lee <holly.lee@gmail.com> 2911 @ Shanghai, China.
// ---------------------------------------------------------------------------------

#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#include "Condition.h"
#include "TimeUtil.h"

namespace turbo {

// --------------------------------------------------------------------------------
// Condition
// --------------------------------------------------------------------------------

int Condition::waitAbsolute(Mutex& mutex, int64_t absoluteTimeOutUs)
{
    if (absoluteTimeOutUs == 0)
        return pthread_cond_wait(&_cond, &(mutex._mutex));

    struct timespec ts;
    TimeUtil::getTimeSpecFromUs(&ts, absoluteTimeOutUs);

    return pthread_cond_timedwait(&_cond, &(mutex._mutex), &ts);
}

int Condition::waitRelative(Mutex& mutex, int64_t timeOutUs)
{
    int64_t nowUs = TimeUtil::getRealTimeUs();
    return waitAbsolute(mutex, nowUs + timeOutUs);
}

} // namespace turbo
