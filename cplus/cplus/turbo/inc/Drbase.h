#ifndef __DrBase_ABSTRACTOR_H__
#define __DrBase_ABSTRACTOR_H__

#include <refcount_ptr.h>
#include <normal_ptr.h>
#include <Mutex.h>





class DrBase
{

public:
    static const char * TAG;
                        
public:
    DrBase():
    _initialized(0){};

    virtual ~DrBase(){};

public:

    virtual void* decode(void *avpkt, int *gotPicture, int *size, int64_t *timeUS) = 0;
    //virtual int decode(void *avpkt, void *data, int *gotPicture, int64_t *timeUs) = 0;
    
    virtual bool init() = 0;
    virtual bool isDecoderReady(){ return _initialized; }
    virtual bool start() = 0;
    virtual void stop() = 0;
    virtual bool pause() = 0;
    virtual bool play() = 0;
    virtual void flushOmxBuffers() = 0;
    virtual void setNativeWindow(void *nativeWindow) = 0;

private:
    bool _initialized;

    
};


#endif
