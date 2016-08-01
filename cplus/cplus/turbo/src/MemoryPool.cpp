// ------------------------------------------------------------------------------
// MemoryPool.cpp
//
// ------------------------------------------------------------------------------

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include <MemoryPool.h>
#include <Mutex.h>
#include "Logger.h"
namespace turbo {

MemoryPool::MemoryPool(size_t bufferSize, size_t poolSize)
{
    _bufferSize = bufferSize;
    _poolSize = poolSize;

    _allocatePool();
}

MemoryPool::~MemoryPool()
{
    _releasePool();
}

void MemoryPool::_allocatePool()
{
    _pPool = (void**)malloc((_poolSize + 1) * sizeof(void*));

    _poolHead = 0, _poolTail = _poolSize; // Full

    for (size_t i = _poolHead; i < _poolTail; i++)
        _pPool[i] = malloc(_bufferSize);

    _pPool[_poolTail] = 0;
}

void MemoryPool::_releasePool()
{
    if (_pPool == 0)
        return;

    for (size_t i = 0; i < _poolSize + 1; i++) {
        // There always has one slot containing NULL, which is the extra one
        if (_pPool[i] != 0)
            free(_pPool[i]);
    }

    free(_pPool);
}

/*
void MemoryPool::setPoolBufferSize(size_t bufferSize, size_t poolSize)
{
     if ( bufferSize == _bufferSize && poolSize == _poolSize )
        return;

     _releasePool();
  
     _bufferSize = bufferSize;
     _poolSize = poolSize;

     _allocatePool();
}

*/

void* MemoryPool::acquire()
{
    assertuc(_bufferSize > 0);
    assertuc(_pPool != 0);

    Mutex::AutoLock lock(_lock);

    void* pBuffer = 0;

    if (isEmpty()) {
        pBuffer = malloc(_bufferSize);
    } else {
#ifdef DEBUG
        assertuc(_pPool[_poolHead] != 0);
#endif
        pBuffer = _pPool[_poolHead];
        _pPool[_poolHead] = 0;
        _poolHead++;
        if (_poolHead > _poolSize)
            _poolHead = 0;
    }

    return pBuffer;
}

void MemoryPool::reclaim(void* pBuffer)
{
    assertuc(_pPool != 0);

    Mutex::AutoLock lock(_lock);

    if (isFull()) {
        free(pBuffer);
    } else {
#ifdef DEBUG
        assertuc(_pPool[_poolTail] == 0);
#endif
        _pPool[_poolTail] = pBuffer;
        _poolTail++;
        if (_poolTail > _poolSize)
            _poolTail = 0;
    }
}

} // namespace turbo
