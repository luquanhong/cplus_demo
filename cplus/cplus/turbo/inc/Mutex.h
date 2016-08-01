// ----------------------------------------------------------------------------------------
// Mutex Class
//
// ----------------------------------------------------------------------------------------
#ifndef __TURBO_MUTEX_H__
#define __TURBO_MUTEX_H__

#include <pthread.h>
#include <stdio.h>

namespace turbo {

// -----------------------------------------------------------------------------------
// Mutex class
// -----------------------------------------------------------------------------------
class Mutex {

public:
    class AutoLock {

    public:
        AutoLock(Mutex& mutex)
            : _mutex(mutex)
        {
            _mutex.lock();
        }
        ~AutoLock()
        {
            _mutex.unlock();
        }

    private:
        Mutex& _mutex;

    }; // class AutoLock

public:
    Mutex()
    {
        pthread_mutex_init(&_mutex, 0);
    }
    ~Mutex()
    {
        pthread_mutex_destroy(&_mutex);
    }

    void lock()
    {
        pthread_mutex_lock(&_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }

    bool tryLock()
    {
        return pthread_mutex_trylock(&_mutex) == 0;
    }

private:
    pthread_mutex_t _mutex;

    friend class Condition;

private:
    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);

}; // class Mutex

} // namespace turbo

#endif // __TURBO_MUTEX_H__
