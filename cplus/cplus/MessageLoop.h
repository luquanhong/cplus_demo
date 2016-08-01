#ifndef __R2_MESSAGE_LOOP_H__
#define __R2_MESSAGE_LOOP_H__

#include <sys/types.h>
#include <Thread.h>
#include <refcount_ptr.h>
#include <Mutex.h>
#include <Condition.h>
//#include <Logger.h>

#include <vector>
using namespace std;
using namespace turbo;

class MessageLoop {

public:
    class Message {

    public:
        virtual ~Message() {}

    public:
        // This will be called in a separated thread.
        virtual void onMessage() = 0;

    }; // class Message

public:
    const static size_t kDefaultPreAllocatedMessagePoolSize = 5;

public:
    // MessageLoop::BreakForStopRoutine is for breaking possible block calls in
    // message handler when we need to stop the loop.
    typedef void (*BreakForStopRoutine)(void* pParam);

public:
    MessageLoop(size_t preAllocatedMessagePoolSize = kDefaultPreAllocatedMessagePoolSize)
        : _messageQueue(preAllocatedMessagePoolSize)
        , _running(false)
        , _detached(false)
        , _maxPendingMessageSize(-1)
    {
#ifdef DEBUG
        // fprintf(stderr, "MessageLoop %p, messageQueue %p\n", this, &_messageQueue);
        //Logger::d("MessageLoop", "MessageLoop %p, messageQueue %p\n", this, &_messageQueue);
        
        cout << __FUNCTION__ << endl;
#endif
    }

    virtual ~MessageLoop()
    {
        stop();
        cout << __FUNCTION__ << endl;
    }

    bool start(bool detache = false);

    // breakForStopRoutine can be null.
    void stop(BreakForStopRoutine breakForStopRoutine = 0, void* pBreakParam = 0);

    // Call this to stop the thread inside itself
    void selfStop();

    void setThreadName(const char* name)
    {
        _threadName = name;
    }

    int getThreadPriority();
    int getThreadPriority(int* pPolicy);

    void setThreadPriority(int newPriority);
    void setThreadPriority(int policy, int newPriority);

    bool threadDetached()
    {
        return _detached;
    }
    void setThreadDetached(bool value){
        _detached = value;
    }
    int detachThread();

    void postMessage(refcount_ptr<Message>& message, bool postToHead = false);

    void clearAllMessages();

    // Stop the message loop thread inside the thread
    // void stopInside();

    void setMaxPendingMessageSize(int size)
    {
        _maxPendingMessageSize = size;
    }

public:
    virtual void* threadRoutine(Thread<MessageLoop>* pThread);
    /*
private:
      class StopMessage : public Message {
    
      public:
            StopMessage(MessageLoop * pLoop) : _pLoop(pLoop) {}
            virtual void onMessage() { _pLoop->_handleStopMessage(); }
        
      private:
            MessageLoop * _pLoop;
          
      }; // class StopMessage
    
      void _handleStopMessage();
*/

private:
    Thread<MessageLoop> _loopThread;

    vector<refcount_ptr<Message> > _messageQueue;
	string _threadName;

    Mutex _messageQueueLock;
    Condition _messageAvailableCondition;

    volatile bool _running;

    

    bool _detached;

protected:
    //by mamk
    //-1 means not skiping, otherwise skip message while queue size exceed this value.
    int _maxPendingMessageSize;

private:
    MessageLoop(const MessageLoop&);
    MessageLoop& operator=(const MessageLoop&);

}; // class MessageLoop


#endif // __R2_MESSAGE_LOOP_H__
