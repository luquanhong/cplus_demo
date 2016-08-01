// -----------------------------------------------------------------------------------
// auto_ptr.h
//
// -----------------------------------------------------------------------------------
#ifndef __TURBO_BASE_AUTO_PTR_H__
#define __TURBO_BASE_AUTO_PTR_H__

namespace turbo {

template <typename T>
class auto_ptr {

public:
    explicit auto_ptr()
        : _ptr(0)
    {
    }
    explicit auto_ptr(T* ptr)
        : _ptr(ptr)
    {
    }
    auto_ptr(const auto_ptr& another)
        : _ptr(another._ptr)
    {
        another._ptr = 0;
    }

    ~auto_ptr()
    {
        if (_ptr != 0)
            delete _ptr;
    }

    T* ptr()
    {
        return _ptr;
    }

    T& operator*()
    {
        return *_ptr;
    }
    T* operator->()
    {
        return _ptr;
    }

    inline auto_ptr& operator=(const auto_ptr& another);
    inline auto_ptr& operator=(T* ptr);

    bool operator==(const T* ptr)
    {
        return _ptr == ptr;
    }
    bool operator==(const auto_ptr& another)
    {
        return _ptr == another._ptr;
    }
    bool operator!=(const T* ptr)
    {
        return !operator==(ptr);
    }
    bool operator!=(const auto_ptr& another)
    {
        return !operator==(another);
    }

private:
    T* _ptr;

}; // class auto_ptr

template <typename T>
auto_ptr<T>& auto_ptr<T>::operator=(const auto_ptr<T>& another)
{
    if (_ptr != 0)
        delete _ptr;

    _ptr = another._ptr;
    another._ptr = 0;

    return this;
}

template <typename T>
auto_ptr<T>& auto_ptr<T>::operator=(T* ptr)
{
    if (_ptr != 0)
        delete _ptr;

    _ptr = ptr;

    return this;
}

} // namespace turbo

#endif // __TURBO_BASE_AUTO_PTR_H__
