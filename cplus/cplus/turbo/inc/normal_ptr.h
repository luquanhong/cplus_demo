// ----------------------------------------------------------------------------------------
// normal_ptr.h
//
// This is a simple wrapper type for plain pointer.
//
// ----------------------------------------------------------------------------------------
#ifndef __TURBO_NORMAL_PTR_H__
#define __TURBO_NORMAL_PTR_H__

#include <refcount_ptr.h>

namespace turbo {

template <typename T>
class normal_ptr {

public:
    normal_ptr()
        : _p(0)
    {
    }
    normal_ptr(T* p)
        : _p(p)
    {
    }
    normal_ptr(const normal_ptr<T>& another)
        : _p(another._p)
    {
    }

    template <typename U>
    normal_ptr(const normal_ptr<U>& another)
        : _p(another.get())
    {
    }

    normal_ptr(const refcount_ptr<T>& rp)
        : _p(rp.get())
    {
    }

    template <typename U>
    normal_ptr(const refcount_ptr<U>& rp)
        : _p(rp.get())
    {
    }

    T* operator->()
    {
        return _p;
    }
    T& operator*()
    {
        return _p;
    }

    T* get() const
    {
        return _p;
    }
    T* ptr() const
    {
        return _p;
    }

    normal_ptr<T>& operator=(const normal_ptr<T>& another)
    {
        _p = another._p;
        return *this;
    }

    template <typename U>
    normal_ptr<T>& operator=(const normal_ptr<U>& another)
    {
        _p = another.get();
        return *this;
    }

    normal_ptr<T>& operator=(T* p)
    {
        _p = p;
        return *this;
    }

    template <typename U>
    normal_ptr<T>& operator=(U* p)
    {
        _p = p;
        return *this;
    }

    normal_ptr<T>& operator=(const refcount_ptr<T>& rp)
    {
        _p = rp.get();
        return *this;
    }

    template <typename U>
    normal_ptr<T>& operator=(const refcount_ptr<U>& rp)
    {
        _p = rp.get();
        return *this;
    }

    bool operator==(const normal_ptr<T>& another)
    {
        return _p == another._p;
    }

    template <typename U>
    bool operator==(const normal_ptr<U>& another)
    {
        return _p == another.get();
    }

    bool operator==(const refcount_ptr<T>& rp)
    {
        return _p == rp._p;
    }

    template <typename U>
    bool operator==(const refcount_ptr<U>& rp)
    {
        return _p == rp.get();
    }

    bool operator!=(const normal_ptr<T>& another)
    {
        return !operator==(another);
    }

    template <typename U>
    bool operator!=(const normal_ptr<U>& another)
    {
        return !operator==(another);
    }

    bool operator!=(const refcount_ptr<T>& rp)
    {
        return !operator==(rp);
    }

    template <typename U>
    bool operator!=(const refcount_ptr<U>& rp)
    {
        return !operator==(rp);
    }

    operator bool() const
    {
        return _p != 0;
    }

private:
    T* _p;

}; // class normal_ptr

} // namespace turbo

#endif // __TURBO_NORMAL_PTR_H__
