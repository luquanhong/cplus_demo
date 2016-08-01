// ----------------------------------------------------------------------------------
// Thread.h
//
// ----------------------------------------------------------------------------------
#ifndef __TURBO_THREAD_H__
#define __TURBO_THREAD_H__

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
//#include <Logger.h>

#ifdef __ANDROID__
#include <sys/prctl.h>
#endif

#include <string>
#include <vector>
using namespace std;

namespace turbo {

// ----------------------------------------------------------------------------------
// Thread class
// ----------------------------------------------------------------------------------
template <typename T>
class Thread {

public:
    explicit Thread()
        : _threadId(0)
        , _threadCreated(false)
        , _threadRunning(false)
        , _pRunnable(0)
        , _threadDetached(false)
    {
    }

    ~Thread()
    {
#ifdef DEBUG
//fprintf(stderr, "Thread destructor id 0x%x, _threadRunning = %d\n", _threadId, _threadRunning);
#endif
        //by mamk
        if (!_threadDetached) {
            //assertuc(!_threadRunning);
        }
    }

public:
    bool start(T* pRunnable, bool detach = false);

    void* join()
    {
        void* value_ptr = 0;
        pthread_join(_threadId, &value_ptr);
        return value_ptr;
    }

    // This must be called inside the thread routine
    void setName(const char* name);
    string& getName() const
    {
        return _name;
    }

    pthread_t getThreadId() const
    {
        return _threadId;
    }

    bool isRunning() const
    {
        return _threadRunning;
    }

    // Priority and Policy
    void setPrioirty(int priority);
    void setPriority(int schedulePolicy, int priority);

    int getPriority();
    int getPriority(int& schedulePolicy);

    // Detach
    // int detach();

private:
    static void* threadRoutine(void* arg);

private:
    pthread_t _threadId;
    bool _threadCreated;
    bool _threadDetached;
    volatile bool _threadRunning;

    string _name;

    T* _pRunnable;

private:
    Thread(const Thread<T>&);
    Thread<T>& operator=(const Thread<T>&);

}; // class Thread

// static
template <typename T>
void* Thread<T>::threadRoutine(void* arg)
{
    Thread<T>* pThread = static_cast<Thread<T>*>(arg);

#ifdef DEBUG
//fprintf(stderr, "Thread::threadRoutine starts. id = 0x%x, _threadRunning = %d\n", (pThread->_threadId), pThread->_threadRunning);
#endif

    pThread->_threadRunning = true;
    void* rv = pThread->_pRunnable->threadRoutine(pThread);
    pThread->_threadRunning = false;

#ifdef DEBUG
//fprintf(stderr, "Thread::threadRoutine ended. id = 0x%x, _threadRunning = %d\n", (pThread->_threadId), pThread->_threadRunning);
#endif

    return rv;
}

template <typename T>
bool Thread<T>::start(T* pRunnable, bool detach)
{
    _pRunnable = pRunnable;
    _threadDetached = detach;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, detach ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);

    _threadCreated = pthread_create(&_threadId, &attr, threadRoutine, this) == 0;

    pthread_attr_destroy(&attr);

    return _threadCreated;
}

template <typename T>
void Thread<T>::setName(const char* name)
{
    _name = name;

#ifdef __ANDROID__
    prctl(PR_SET_NAME, (unsigned long)name, 0, 0, 0);
#endif

#ifdef __APPLE__
    pthread_setname_np(name);
#endif
}

} // namespace turbo

#endif // __TURBO_THREAD_H__
