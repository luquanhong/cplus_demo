// ----------------------------------------------------------------------------------
// MessageLoop.cpp
//
// ----------------------------------------------------------------------------------
#include <TimeUtil.h>

#include "MessageLoop.h"


using namespace turbo;

bool MessageLoop::start(bool detach)
{
    Mutex::AutoLock lock(_messageQueueLock);

    if (_running)
        return true;

    _running = true;
    return _loopThread.start(this, detach);
}

// Should be called outside
void MessageLoop::stop(BreakForStopRoutine breakForStopRoutine /* = 0 */, void* pBreakParam /* = 0 */)
{
    do {
        Mutex::AutoLock lock(_messageQueueLock);

        if (!_running)
            return;

#ifdef DEBUG
        // fprintf(stderr, "MessageLoop::stop %p (thread %p) set _running to false now\n", this, _loopThread.getThreadId());
        //Logger::d("MessageLoop", "T(%s) MessageLoop::stop %p (thread %p) set _running to false now\n", _threadName.c_str(), this, _loopThread.getThreadId());
#endif

        _running = false;
        _messageAvailableCondition.signal();

    } while (0);

// This should break all possible blocks.
#if defined(DEBUG) && defined(__ANDROID__)
    //Logger::d("MessageLoop", "T(%s) MessageLoop::stop() %p, Will call breakForStop routime at %p param %p\n", _threadName.c_str(), this, breakForStopRoutine, pBreakParam);
#endif

    if (breakForStopRoutine)
        breakForStopRoutine(pBreakParam);

#if defined(DEBUG) && defined(__ANDROID__)
    //Logger::d("MessageLoop", "T(%s) MessageLoop::stop() %p, Join thread now\n", _threadName.c_str(), this);
#endif

    // Wait for thread.
    if (!_detached) {
#ifdef DEBUG
        //Logger::d("MessageLoop", "T(%s) MessageLoop::stop %p thread %p start join.\n", _threadName.c_str(), this, _loopThread.getThreadId());
#endif // DEBUG
        _loopThread.join();
    }

#ifdef DEBUG
    // fprintf(stderr, "MessageLoop::stop %p thread %p joined.\n", this, _loopThread.getThreadId());
    //Logger::d("MessageLoop", "T(%s) MessageLoop::stop %p thread %p joined.\n", _threadName.c_str(), this, _loopThread.getThreadId());
#endif
}

void MessageLoop::selfStop()
{
    _running = false;

    // detach itself
    detachThread();

// OK
#ifdef DEBUG
    //Logger::d("MessageLoop", "T(%s) MessageLoop::selfStop() %p, thread Id %p", _threadName.c_str(), this, _loopThread.getThreadId());
#endif
}

void MessageLoop::postMessage(refcount_ptr<Message>& message, bool postToHead)
{
    Mutex::AutoLock lock(_messageQueueLock);

    if (postToHead)
        _messageQueue.insert(_messageQueue.begin(), message);
    else
        _messageQueue.push_back(message);

#ifdef DEBUG
// fprintf(stderr, "messageLoop %p queue %p size = %lu\n", this, &_messageQueue, _messageQueue.size());
#endif

    _messageAvailableCondition.signal();
}

void MessageLoop::clearAllMessages()
{
    Mutex::AutoLock lock(_messageQueueLock);

    _messageQueue.clear();
}

/*    
void MessageLoop::_handleStopMessage()
{
     Mutex::AutoLock lock(_messageQueueLock);
    
     _running = false;
}
*/

int MessageLoop::getThreadPriority(int* pPolicy)
{
    int policy;
    struct sched_param param;

    pthread_getschedparam(_loopThread.getThreadId(), &policy, &param);
    if (pPolicy != 0)
        *pPolicy = policy;

    return param.sched_priority;
}

int MessageLoop::getThreadPriority()
{
    return getThreadPriority(0);
}

void MessageLoop::setThreadPriority(int newPriority)
{
    int policy;
    struct sched_param param;

    pthread_getschedparam(_loopThread.getThreadId(), &policy, &param);
    param.sched_priority = newPriority;

    pthread_setschedparam(_loopThread.getThreadId(), policy, &param);
}

void MessageLoop::setThreadPriority(int policy, int newPriority)
{
    struct sched_param param;

    pthread_getschedparam(_loopThread.getThreadId(), &policy, &param);
    param.sched_priority = newPriority;

    pthread_setschedparam(_loopThread.getThreadId(), policy, &param);
}

int MessageLoop::detachThread()
{
    if (_detached)
        return -1;

    int rv = pthread_detach(_loopThread.getThreadId());
    _detached = rv == 0;

    return rv;
}

// The thread routine for message loop.
void* MessageLoop::threadRoutine(Thread<MessageLoop>* pThread)
{
    pThread->setName(_threadName.c_str());

    while (_running) {

        refcount_ptr<Message> message;

        do {
            Mutex::AutoLock lock(_messageQueueLock);

            // Running: wait for message incoming
            while (_messageQueue.size() == 0 && _running)
                _messageAvailableCondition.wait(_messageQueueLock);

            // Stop operation may signal messageAvailableCondition, too
            if (!_running)
                break;

            // We have message
            while (_maxPendingMessageSize != -1 && _messageQueue.size() > _maxPendingMessageSize)
                _messageQueue.erase(_messageQueue.begin());

            vector<refcount_ptr<Message> >::iterator it = _messageQueue.begin();
            message = *it;
            _messageQueue.erase(it);

        } while (0);

        if (message && _running) {
            message->onMessage();
        }

    } // while ( _running )

    return 0;
}

