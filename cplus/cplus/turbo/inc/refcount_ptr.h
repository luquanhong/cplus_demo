// ----------------------------------------------------------------------------------------
// refcount_ptr.h
//
// Refreence counted smart pointer
//
// ----------------------------------------------------------------------------------------
#ifndef __TURBO_REFCOUNT_PTR_H__
#define __TURBO_REFCOUNT_PTR_H__

#include <atomic_ops.h>
#include <assert.h>
#include "Logger.h"
#include <iostream>

using namespace std;

namespace turbo {

// RefCount implementation
class __refcount {

public:
    __refcount()
        : _refCount(0)
    {
    }
    __refcount(const __refcount& another);

    int addRef()
    {
        cout << __FUNCTION__  << this << endl;
        return atomicIncrement32(&_refCount);
    }
    int release()
    {
        cout << __FUNCTION__  << this << endl;
        return atomicDecrement32(&_refCount);
    }

    int refCount()
    {
        return _refCount;
    }

private:
    volatile /*int*/ atomic_int32_t _refCount;

}; // class RefCount

/**
 *  Reference Counted Smart Pointer
 */
template <typename T>
class refcount_ptr {

public:
    refcount_ptr()
        : _ptr(0)
        , _refCount(0)
    {
    }

    refcount_ptr(T* ptr)
        : _ptr(ptr)
    {
        if (ptr == 0) {
            _refCount = 0;
        } else {
            _refCount = new __refcount();
            _refCount->addRef();
        }
    }

    template <typename U>
    refcount_ptr(U* ptr)
        : _ptr(ptr)
    {
        if (ptr == 0) {
            _refCount = 0;
        } else {
            _refCount = new __refcount();
            _refCount->addRef();
        }
    }

    refcount_ptr(const refcount_ptr<T>& another)
        : _ptr(another.ptr())
        , _refCount(another._refCount)
    {
        if (_refCount != 0)
            _refCount->addRef();
    }

    refcount_ptr(refcount_ptr<T>& another)
        : _ptr(another.ptr())
        , _refCount(another._refCount)
    {
        if (_refCount != 0)
            _refCount->addRef();
    }


    template <typename U>
    refcount_ptr(const refcount_ptr<U>& another)
        : _ptr(another.ptr())
        , _refCount(another.refcount())
    {
        if (_refCount != 0)
            _refCount->addRef();
    }


    template <typename U>
    refcount_ptr(refcount_ptr<U>& another)
        : _ptr(another.ptr())
        , _refCount(another.refcount())
    {
        if (_refCount != 0)
            _refCount->addRef();
    }

    ~refcount_ptr()
    {
        _cleanupIfLastInstance();
    }

    T& operator*() const
    {
        return *_ptr;
    }
    T* operator->() const
    {
        return _ptr;
    }
    T* ptr() const
    {
        return _ptr;
    }
    T* get() const
    {
        return _ptr;
    }
    operator T*() const
    {
        return _ptr;
    }

    operator bool()
    {
        return _ptr != 0;
    }

    refcount_ptr<T>& operator=(const refcount_ptr<T>& another)
    {
        _cleanupIfLastInstance();

        _ptr = another._ptr;
        _refCount = another._refCount;

        if (_refCount != 0)
            _refCount->addRef();

        return *this;
    }

    template <typename U>
    refcount_ptr<T>& operator=(const refcount_ptr<U>& another)
    {
        _cleanupIfLastInstance();

        _ptr = another.ptr();
        _refCount = another.refcount();

        if (_refCount != 0)
            _refCount->addRef();

        return *this;
    }

    refcount_ptr<T>& operator=(T* ptr)
    {
        _cleanupIfLastInstance();

        _ptr = ptr;

        if (ptr == 0) {
            _refCount = 0;
        } else {
            _refCount = new __refcount();
            _refCount->addRef();
        }

        return *this;
    }

    template <typename U>
    refcount_ptr<T>& operator=(U* ptr)
    {
        _cleanupIfLastInstance();

        _ptr = ptr;

        if (ptr == 0) {
            _refCount = 0;
        } else {
            _refCount = new __refcount();
            _refCount->addRef();
        }

        return *this;
    }

    bool operator==(const refcount_ptr<T>& another)
    {
        return _ptr == another._ptr;
    }

    template <typename U>
    bool operator==(const refcount_ptr<U>& another)
    {
        return _ptr == another.ptr();
    }

    bool operator==(const T* ptr)
    {
        return _ptr == ptr;
    }

    template <typename U>
    bool operator==(const U* ptr)
    {
        return _ptr == ptr;
    }

    bool operator!=(const refcount_ptr<T>& another)
    {
        return !operator==(another);
    }

    template <typename U>
    bool operator!=(const refcount_ptr<U>& another)
    {
        return !operator==(another);
    }

    bool operator!=(const T* ptr)
    {
        return !operator==(ptr);
    }

    template <typename U>
    bool operator!=(const U* ptr)
    {
        return !operator==(ptr);
    }

    // Internal use
    __refcount* refcount() const
    {
        return _refCount;
    }

private:
    T* _ptr;
    __refcount* _refCount;

private:
    void _cleanupIfLastInstance()
    {
        if (_ptr == 0) {
            //assertuc(_refCount == 0);
            return;
        }

        
        if (_refCount != 0 && _refCount->release() == 0) {
            cout << __FUNCTION__  << this << endl;
            delete _ptr;
            delete _refCount;
        }
    }

}; // class refcount_ptr


} // namespace turbo

#endif // __TURBO_REFCOUNT_PTR_H__
