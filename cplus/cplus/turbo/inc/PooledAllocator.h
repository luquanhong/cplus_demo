// -------------------------------------------------------------------------------------------
// PooledAllocator.h
//
// PooledAllocator pre-allocates a specified number of objects for later allocation. It
// provides method to allocate object from this pool and method to reclaim objects into
// the pool.
//
// When pre-allocated objects are not enough, it will call system's new() method to
// constructs object and such an object will also be put into pool when release method
// is called to reclaim the object.
//
// Requirements for the object in pool:
//
//   1) The object must have a default null constructor.
//   2) The cost of assignment is not high
//
// -------------------------------------------------------------------------------------------
#ifndef __TURBO_POOLED_ALLOCATOR_H__
#define __TURBO_POOLED_ALLOCATOR_H__

#include <sys/types.h>
#include "Logger.h"

#include "LinkedListItem.h"

namespace turbo {

// ListItem Pool
template <typename T>
struct PooledAllocator {

public:
    PooledAllocator(size_t poolSize)
        : _count(0)
        , _poolSize(poolSize)
        , _overflowCount(0)
    {
        _allocatePool();
    }

    ~PooledAllocator();

    size_t getPoolSize()
    {
        return _poolSize;
    }

    LinkedListItem<T>* allocate();
    LinkedListItem<T>* allocate(const T& t);

    void release(LinkedListItem<T>* pItem)
    {
        *pItem = LinkedListItem<T>();
        _put(pItem);
    }

    // This method can be called when poolSize is 0 only
    void setPoolSize(size_t poolSize)
    {
        assertuc(_poolSize == 0);

        _poolSize = poolSize;
        _allocatePool();
    }

private:
    LinkedListItem<T>* _pItemsHead;
    LinkedListItem<T>* _pItemsTail;
    size_t _count;
    size_t _poolSize;
    size_t _overflowCount;

private:
    void _put(LinkedListItem<T>* pItem);

    LinkedListItem<T>* _get();

    void _allocatePool()
    {
        for (size_t i = 0; i < _poolSize; i++) {
            LinkedListItem<T>* pItem = new LinkedListItem<T>();
            _put(pItem);
        }
    }

private:
    PooledAllocator(const PooledAllocator&);
    PooledAllocator& operator=(const PooledAllocator&);

}; // struct PooledAllocator

template <typename T>
PooledAllocator<T>::~PooledAllocator()
{
    LinkedListItem<T>* pItem = _get();
    while (pItem != 0) {
        delete pItem;
        pItem = _get();
    }
}

template <typename T>
LinkedListItem<T>* PooledAllocator<T>::allocate()
{
    if (_count == 0) {
        _overflowCount++;
        _poolSize++;
#ifdef DEBUG
        fprintf(stderr, "allocator %p: overflowCount = %lu\n", this, _overflowCount);
#endif
        return new LinkedListItem<T>();
    } else {
        return _get();
    }
}

template <typename T>
LinkedListItem<T>* PooledAllocator<T>::allocate(const T& t)
{
    if (_count == 0) {
        _overflowCount++;
        _poolSize++;
#ifdef DEBUG
        fprintf(stderr, "allocator %p: overflowCount = %u\n", this, (unsigned)_overflowCount);
#endif
        return new LinkedListItem<T>(t);
    }

    LinkedListItem<T>* pItem = _get();
    *pItem = LinkedListItem<T>(t);
    return pItem;
}

template <typename T>
void PooledAllocator<T>::_put(LinkedListItem<T>* pItem)
{
    if (_count == 0) {
        _pItemsHead = _pItemsTail = pItem;
        pItem->pPrev = pItem->pNext = 0;
    } else {
        pItem->pPrev = _pItemsTail;
        pItem->pNext = 0;
        _pItemsTail->pNext = pItem;
        _pItemsTail = pItem;
    }

    _count++;
}

template <typename T>
LinkedListItem<T>* PooledAllocator<T>::_get()
{
    if (_count == 0)
        return 0;

    LinkedListItem<T>* pItem = _pItemsHead;
    _pItemsHead = pItem->pNext;
    pItem->pPrev = pItem->pNext = 0;

    _count--;

    return pItem;
}

} // namespace turbo

#endif // __TURBO_POOLED_ALLOCATOR_H__
