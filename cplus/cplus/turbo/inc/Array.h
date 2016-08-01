// -----------------------------------------------------------------------------------------
// Array
//
// -----------------------------------------------------------------------------------------
#ifndef __TURBO_ARRAY_H__
#define __TURBO_ARRAY_H__

#include <sys/types.h>

namespace turbo {

template <typename T>
class Array {

public:
    inline explicit Array(size_t initSize = kDefaultInitSize, size_t growSize = kDefaultGrowSize);
    inline Array(const Array<T>& another);
    inline ~Array();

public:
    inline Array& operator=(const Array<T>& another);

    T operator[](size_t index) const
    {
        return _pElements[index];
    }
    T& operator[](size_t index)
    {
        return _pElements[index];
    }

    operator bool() const
    {
        return _pElements != 0;
    }

    size_t count() const
    {
        return _count;
    }

    inline void add(const T&);
    inline void add(const Array<T>& another);

    inline void insert(size_t index, const T&);

    inline void remove(size_t index, size_t number = 1);
    inline void clear();

    inline ssize_t find(const T&);
    inline bool find(const T&, size_t&);

    template <typename U>
    bool find(const U& value, bool (*predicate)(const U&, const T&), size_t& index)
    {
        for (size_t i = 0; i < _count; i++) {
            if (predicate(value, _pElements[i])) {
                index = i;
                return true;
            }
        }

        return false;
    }

private:
    T* _pElements;
    size_t _count;
    size_t _capacity;
    size_t _growSize;

private:
    bool _grow(size_t numberOfElementsToAdd = 1);
    inline void _shrinkIfPossible();

private:
    const static size_t kDefaultInitSize = 10;
    const static size_t kDefaultGrowSize = 10;

}; // class Array

// inline
template <typename T>
Array<T>::Array(size_t initSize, size_t growSize)
    : _pElements(0)
    , _count(0)
    , _capacity(initSize)
    , _growSize(growSize)
{
    _pElements = new T[initSize];
}

// inline
template <typename T>
Array<T>::Array(const Array<T>& another)
    : _pElements(new T[another._capacity])
    , _count(another._count)
    , _capacity(another._capacity)
    , _growSize(another._growSize)
{
    for (size_t i = 0; i < _count; i++)
        _pElements[i] = another._pElements[i];
}

// inline
template <typename T>
Array<T>::~Array()
{
    if (_pElements != 0)
        delete[] _pElements;
}

// inline
template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& another)
{
    _pElements = new T[another._capacity];
    _count = another._count;
    _capacity = another._capacity;
    _growSize = another.growSize;

    for (size_t i = 0; i < _count; i++)
        _pElements[i] = another._pElements[i];

    return *this;
}

// inline
template <typename T>
void Array<T>::add(const T& newElement)
{
    if (_grow(1) == false)
        return;

    _pElements[_count++] = newElement;
}

// inline
template <typename T>
void Array<T>::add(const Array<T>& another)
{
    if (_grow(another._count) == false)
        return;

    for (size_t i = _count; i < _count + another._count; i++)
        _pElements[i] = another._pElements[i - _count];

    _count += another._count;
}

// inline
template <typename T>
void Array<T>::insert(size_t index, const T& item)
{
    if (index > _count)
        return;

    if (index == _count) {
        add(item);
        return;
    }

    if (_grow(1) == false)
        return;

    // Move and insert
    for (size_t i = _count - 1; i >= index; i--)
        _pElements[i + 1] = _pElements[i];

    _pElements[index] = item;

    _count++;
}

// inline
template <typename T>
void Array<T>::remove(size_t index, size_t number)
{
    if (index >= _count || number == 0)
        return;

    if (index + number > _count)
        number = _count - index;

    for (size_t i = index; i < _count - number; i++)
        _pElements[i] = _pElements[i + number];

    for (size_t i = _count - number; i < _count; i++)
        _pElements[i] = T();

    _count -= number;

    _shrinkIfPossible();
}

// inline
template <typename T>
void Array<T>::clear()
{
    for (size_t i = 0; i < _count; i++)
        _pElements[i] = T();

    _count = 0;
}

// inline
template <typename T>
ssize_t Array<T>::find(const T& item)
{
    for (size_t i = 0; i < _count; i++) {
        if (_pElements[i] == item)
            return (ssize_t)i;
    }

    return -1;
}

// inline
template <typename T>
bool Array<T>::find(const T& item, size_t& index)
{
    for (size_t i = 0; i < _count; i++) {
        if (_pElements[i] == item) {
            index = i;
            return true;
        }
    }

    return false;
}

// inline
template <typename T>
bool Array<T>::_grow(size_t numberOfNewElementsToAdd)
{
    if (_count + numberOfNewElementsToAdd <= _capacity)
        return true;

    size_t growSize = _count + numberOfNewElementsToAdd - _capacity;
    growSize = (growSize + (_growSize - 1) / _growSize) * _growSize;

    T* pTemp = new T[_capacity + growSize];
    if (pTemp == 0)
        return false;

    for (size_t i = 0; i < _count; i++)
        pTemp[i] = _pElements[i];

    delete[] _pElements;
    _pElements = pTemp;

    _capacity += growSize;

    return true;
}

// inline
template <typename T>
void Array<T>::_shrinkIfPossible()
{
    // Do we really need this?
}

} // namespace turbo

#endif // __TURBO_ARRAY_H__
