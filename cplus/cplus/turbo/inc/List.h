// ----------------------------------------------------------------------------------------------
// List.h
//
// List implements a linked list
//
// ----------------------------------------------------------------------------------------------
#ifndef __TURBO_LIST_H__
#define __TURBO_LIST_H__

#include <sys/types.h>
#include <stdio.h>

#include "LinkedListItem.h"
#include "PooledAllocator.h"

namespace turbo {

// T is item type
template <typename T>
class List {

public:
    const static size_t kListDefaultAllocatorPoolSize = 10;

public:
    explicit List(size_t listItemPoolSize = kListDefaultAllocatorPoolSize)
        : _count(0)
        , _pHead(0)
        , _pTail(0)
        , _allocator(listItemPoolSize)
    {
#ifdef DEBUG
//fprintf(stderr, "List %p: allocator %p has pool size %ld\n", this, &_allocator, listItemPoolSize);
#endif
    }

    ~List();

private:
    // Disable those
    List(const List<T>&);
    List& operator=(const List<T>&);

public:
    // Iterator
    template <typename U>
    struct ListIterator {

    public:
        U& operator*()
        {
            return _pItem->item;
        }
        U* operator->()
        {
            return _pItem == 0 ? 0 : &(_pItem->item);
        }
        ListIterator& operator++()
        {
            _pItem = _pItem->pNext;
            return *this;
        }
        ListIterator& operator--()
        {
            _pItem = _pItem->pPrev;
            return *this;
        }

        bool operator==(const ListIterator& another)
        {
            return _pItem == another._pItem;
        }
        bool operator!=(const ListIterator& another)
        {
            return !operator==(another);
        }

    private:
        ListIterator()
            : _pItem(0)
        {
        }
        ListIterator(LinkedListItem<U>* pItem)
            : _pItem(pItem)
        {
        }

    private:
        LinkedListItem<U>* _pItem;

        friend class List<U>;

    }; // struct iterator

private:
    size_t _count;

    LinkedListItem<T>* _pHead; // empty: pHead == pTail == 0
    LinkedListItem<T>* _pTail; //

    // ListItemAllocator<T> _allocator;
    PooledAllocator<T> _allocator;

public:
    size_t count() const
    {
        return _count;
    }
    size_t size() const
    {
        return _count;
    }

    bool isEmpty() const
    {
        return _count == 0;
    }

    typedef ListIterator<T> iterator;

    iterator begin()
    {
        return iterator(_pHead);
    }
    iterator end()
    {
        return iterator();
    }
    iterator rbegin()
    {
        return iterator(_pTail);
    }
    iterator rend()
    {
        return iterator();
    }

    // insert before *it. Keep iterator same.
    void insert(iterator it, const T& item);
    void insertHead(const T& item)
    {
        insert(begin(), item);
    }
    void append(const T& item)
    {
        insert(end(), item);
    }

    T pop()
    {
        iterator it = begin();
        T t = *it;
        remove(it);
        return t;
    }

    void push(const T& t)
    {
        append(t);
    }

    // remove *iterator. Move iterator to next.
    void remove(iterator& it);

    // Clear the list
    void clear();

    // Set Pool size
    void setPoolSize(size_t poolSize)
    {
        _allocator.setPoolSize(poolSize);
#ifdef DEBUG
//fprintf(stderr, "List %p set pool size to %lu\n", this, poolSize);
#endif
    }

}; // class List

/**
 *  Insert an new item before *it. Keep iterator same. To insert an item to 
 *  list head, using insert(begin(), item); to insert an item to tail, using
 *  insert(end(), item)
 */
template <typename T>
void List<T>::insert(List<T>::iterator it, const T& item)
{
    // ListItem<T> * pNewItem = new ListItem<T>(item);
    LinkedListItem<T>* pNewItem = _allocator.allocate(item);

    // List is empty...
    if (_pHead == 0) {
        _pHead = _pTail = pNewItem;
    }
    // Not empty: begin()
    else if (it._pItem == _pHead) {
        pNewItem->pNext = _pHead;
        _pHead->pPrev = pNewItem;
        _pHead = pNewItem;
        pNewItem->pPrev = 0;
    }
    // Not empty: end()
    else if (it == end()) {
        _pTail->pNext = pNewItem;
        pNewItem->pPrev = _pTail;
        _pTail = pNewItem;
    }
    // Other
    else {
        pNewItem->pPrev = it._pItem->pPrev;
        pNewItem->pNext = it._pItem;
        pNewItem->pPrev->pNext = pNewItem;
        pNewItem->pNext->pPrev = pNewItem;
    }

    _count++;
}

/**
 *  Remove item of iterator. The iterator would be moved to next. 
 *  If iterator is already end(), do nothing
 */
template <typename T>
void List<T>::remove(List<T>::iterator& it)
{
    if (it == end())
        return;

    LinkedListItem<T>* pItem = it._pItem;
    ++it;

    if (pItem == _pHead)
        _pHead = pItem->pNext;

    if (pItem == _pTail)
        _pTail = pItem->pPrev;

    if (pItem->pPrev != 0)
        pItem->pPrev->pNext = pItem->pNext;

    if (pItem->pNext != 0)
        pItem->pNext->pPrev = pItem->pPrev;

    _count--;

    // delete pItem;
    _allocator.release(pItem);
}

/**
 *  Clear the queue
 */
template <typename T>
void List<T>::clear()
{
    for (iterator it = begin(); it != end();)
        remove(it);
}

/**
 *  Destructor
 */
template <typename T>
List<T>::~List()
{
    LinkedListItem<T>* p = _pHead;
    LinkedListItem<T>* q = 0;
    while (p != 0) {
        q = p->pNext;
        // delete p;
        _allocator.release(p);
        p = q;
    }
}

} // namespac turbo

#endif // __TURBO_LIST_H__
