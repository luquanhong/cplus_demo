// -----------------------------------------------------------------------------------------
// SortedArray
//
// -----------------------------------------------------------------------------------------
#ifndef __TURBO_SORTED_ARRAY_H__
#define __TURBO_SORTED_ARRAY_H__

#include <sys/types.h>

#include <Array.h>

namespace turbo {

template <typename T>
class SortedArray {

public:
    explicit SortedArray(size_t initSize = kDefaultInitSize, size_t growSize = kDefaultGrowSize)
        : _array(initSize, growSize)
    {
    }
    SortedArray(const SortedArray<T>& another)
        : _array(another._array)
    {
    }
    ~SortedArray() {}

public:
    SortedArray& operator=(const SortedArray<T>& another)
    {
        _array = another._array;
    }

    // Only read permitted for element
    T& operator[](size_t index) const
    {
        return _array[index];
    }

    bool operator() const
    {
        return _array.operator();
    }

    size_t count() const
    {
        return _array.count();
    }
    size_t size() const
    {
        return _array.size();
    }

    void add(const T&);

    void remove(size_t index, size_t number = 1)
    {
        return _array.remove(index, number);
    }

    // Return appropriate index if not found
    bool find(const T&, size_t& index);

private:
    Array _array;

private:
    static const size_t kDefaultInitSize = 10;
    static const size_t kDefaultGrowSize = 10;

}; // class SortedArray

template <typename T>
void SortedArray::add(const T& item)
{
    size_t index;
    find(item, index);

    _array.insert(index, item);
}

template <typename T>
bool SortedArray::find(const T& item, size_t& index)
{
    bool found = false;

    for (index = 0; index < _array.count(); index++) {
        if (item == _array[index]) {
            found = true;
            break;
        } else if (item > _array[index]) {
            break;
        }
    }

    return found;
}

} // namespace turbo

#endif // __TURBO_SORTED_ARRAY_H__
