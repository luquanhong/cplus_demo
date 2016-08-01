// ---------------------------------------------------------------------------------------
// MmeoryPool.h
//
// MemoryPool maintains pre-allocated fixed size memory buffers.
//
// ---------------------------------------------------------------------------------------
#ifndef __TURBO_MEMORY_POOL_H__
#define __TURBO_MEMORY_POOL_H__

#include <sys/types.h>

#include <Mutex.h>

namespace turbo {

class MemoryPool {

public:
    const static size_t kDefaultPoolSize = 256;

public:
    MemoryPool(size_t bufferSize, size_t poolSize = kDefaultPoolSize);
    ~MemoryPool();

    // void setPoolBufferSize(size_t bufferSize, size_t poolSize);

    void* acquire();
    void reclaim(void*);

    size_t getBufferSize()
    {
        return _bufferSize;
    }
    size_t getPoolSize()
    {
        return _poolSize;
    }
    size_t getPoolItemCount()
    {
        return _poolTail >= _poolHead ? _poolTail - _poolHead : _poolSize - _poolHead + 1 + _poolTail;
    }

    bool isEmpty()
    {
        return _poolHead == _poolTail;
    }
    bool isFull()
    {
        return (_poolTail == _poolHead - 1) || (_poolHead == 0 && _poolTail == _poolSize);
    }

private:
    void** _pPool; // A fixed-size ring buffer
    size_t _poolHead, _poolTail;

    size_t _bufferSize;
    size_t _poolSize;

    Mutex _lock; // We need a lock
private:
    void _allocatePool();
    void _releasePool();

}; // class MemoryPool

} // namespace turbo

#endif // __TURBO_MEMORY_POOL_H__
