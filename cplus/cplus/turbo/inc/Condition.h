// -----------------------------------------------------------------------------------
// Condition class
//
// -----------------------------------------------------------------------------------
#ifndef __TURBO_CONDITION_H__
#define __TURBO_CONDITION_H__

#include <pthread.h>
#include <stdint.h>
#include <errno.h>

#include "Mutex.h"

namespace turbo {

class Condition {

public:
    Condition()
    {
        pthread_cond_init(&_cond, 0);
    }
    ~Condition()
    {
        pthread_cond_destroy(&_cond);
    }

    enum {
        kWaitConditionSatisfied = 0,
        kWaitTimedOut = ETIMEDOUT
    };

    int wait(Mutex& mutex)
    {
        return pthread_cond_wait(&_cond, &mutex._mutex);
    }

    // We need return values from waitXXX() calls to see whether we are interrupted or timedout.
    int waitAbsolute(Mutex& mutex, int64_t absoluteTimeOutUs = 0);
    int waitRelative(Mutex& mutex, int64_t timeOutUs = 0);

    void signal()
    {
        pthread_cond_signal(&_cond);
    }
    void broadcast()
    {
        pthread_cond_broadcast(&_cond);
    }

private:
    pthread_cond_t _cond;

private:
    // Disable them.
    Condition(const Condition&);
    Condition& operator=(const Condition&);

}; // class Condition

} // namespace turboe

#endif // __TURBO_CONDITION_H__
