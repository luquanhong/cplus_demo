// -------------------------------------------------------------------------
// StaticArray.h
//
// -------------------------------------------------------------------------
#ifndef __TURBO_STATIC_ARRAY_H__
#define __TURBO_STATIC_ARRAY_H__

#include <sys/types.h>

namespace turbo {

template <typename T>
class StaticArray {

public:
    explicit StaticArray(size_t size)
        : _size(size)
        , _pElements(new T[size])
    {
    }

    StaticArray(const StaticArray& another)
        : _size(another._size)
        , _pElements(new T[another._size])
    {
        for (size_t i = 0; i < _size; i++)
            _pElements[i] = another._pElements[i];
    }

    ~StaticArray()
    {
        delete[] _pElements;
    }

    StaticArray& operator=(const StaticArray& another)
    {
        _size = aother._size;
        _pElements = new T[_size];

        for (size_t i = 0; i < _size; i++)
            _pElements[i] = another._pElements[i];
    }

public:
    size_t size() const
    {
        return _size;
    }
    size_t count() const
    {
        return _size;
    }

    T operator[](size_t index) const
    {
        return _pElements[index];
    }
    T& operator[](size_t index)
    {
        return _pElements[index];
    }

public:
private:
    T* _pElements;
    size_t _size;

private:
    StaticArray();

}; // class StaticArray

} // namespace turbo

#endif // __TURBO_STATIC_ARRAY_H__
