
#ifndef __TURBO_THREAD_TIMER_H__
#define __TURBO_THREAD_TIMER_H__

#include <unistd.h>

#include "Thread.h"
#include "TimeUtil.h"

namespace turbo {

class ThreadTimer {

    typedef void (*Timerfunc)(void* p);
    typedef Timerfunc TimerHandler;

public:
    ThreadTimer()
        : _handler(0)
        , _paramrter(0)
        , _interval(0)
        , _timeout(0)
        , _running(0)
    {
    }

    virtual ~ThreadTimer()
    {
        if(_running)
            stop();

        _thread.join();
    }

    bool start()
    {
        _running = true;
        return _thread.start(this);
    }

    void stop()
    {
        Mutex::AutoLock lock(_lock);
        
        if (!_running)
            return ;
        
        _running = false;
        _availableCondition.signal();
    }

    void setInterval(TimerHandler handler, void* p, int millisecond)
    {
        Mutex::AutoLock lock(_lock);

        _handler = handler;
        _paramrter = p;
        _interval = millisecond * TimeUtil::kUsPerMs;
        
        _availableCondition.signal();
    }

    void clearInterval()
    {
        Mutex::AutoLock lock(_lock);

        _handler = 0;
        _paramrter = 0;
        _interval = _timeout = 0;
    }

    void setTimeout(TimerHandler handler, void* p, int millisecond)
    {
        Mutex::AutoLock lock(_lock);

        _handler = handler;
        _paramrter = p;
        _timeout = millisecond * TimeUtil::kUsPerMs;
        
        _availableCondition.signal();
    }
    
    void clearTimeout()
    {
        Mutex::AutoLock lock(_lock);
        
        _handler = 0;
        _paramrter = 0;
        _interval = _timeout = 0;
    }

    void setThreadName(const char* name)
    {
        _threadName = name;
    }

    // The thread routine for message loop.
    virtual void* threadRoutine(Thread<ThreadTimer>* pThread)
    {
        pThread->setName(_threadName.c_str());

        int64_t now = TimeUtil::getRealTimeUs();
        int64_t last_time = now;

        while (_running) {

            TimerHandler handler = NULL;
            void* paramrter = NULL;
            
            do {
                Mutex::AutoLock lock(_lock);
                
                while (_handler == 0 && _running)
                    _availableCondition.wait(_lock);
                
                if (!_running || _handler == 0)
                    break;
                
                now = TimeUtil::getRealTimeUs();
                
                if (_interval && now - last_time >= _interval && _running) {
                    handler = _handler;
                    paramrter = _paramrter;
                    last_time = now;
                    
                }
                else if (_interval && now - last_time < _interval && _running) {
                    _availableCondition.waitRelative(_lock, max(_interval + last_time - now, 1LL));
                }
                
                if (_timeout && now - last_time >= _timeout && _running) {
                    handler = _handler;
                    paramrter = _paramrter;
                    break;
                }
                else if (_timeout && now - last_time < _timeout && _running) {
                    _availableCondition.waitRelative(_lock, max(_interval + last_time - now, 1LL));
                }
                
            } while(0);
            
            if (handler && paramrter && _running) {
                (*handler)(paramrter);
            }

        } // while (_running)

        return 0;
    }

private:
    Thread<ThreadTimer> _thread;
    string _threadName;
    volatile bool _running;

    TimerHandler _handler;
    void* _paramrter;
    int64_t _interval;
    int64_t _timeout;
    
    Mutex _lock;
    Condition _availableCondition;

private:
    ThreadTimer(const ThreadTimer&);
    ThreadTimer& operator=(const ThreadTimer&);

}; // class ThreadTimer

} //namespace turbo

#endif //__TURBO_THREAD_TIMER_H__
