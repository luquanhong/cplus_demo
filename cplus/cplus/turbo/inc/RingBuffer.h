// ----------------------------------------------------------------------------------------------
// RingBuffer
//
// ----------------------------------------------------------------------------------------------
#ifndef __TURBO_RING_BUFFER_H__
#define __TURBO_RING_BUFFER_H__

#include <sys/types.h>

namespace turbo {

/**
 *  Requirements:
 *   1) T supports empty constructor and has almost no extra cost
 *   2) T supports ==
 */
template <typename T>
class RingBuffer {

public:
    RingBuffer(size_t capacity)
        : _capacity(capacity)
        , _pBuffer(new T[capacity + 1])
        , _putIndex(0)
        , _getIndex(0)
    {
    }
    ~RingBuffer()
    {
        delete[] _pBuffer;
    }

    bool push(const T&);
    bool pop(T& t);

    size_t count()
    {
        return _putIndex == _getIndex ? 0 : (_putIndex > _getIndex ? (_putIndex - _getIndex) : _capacity - (_getIndex - _pitIndex) + 1);
    }
    size_t size()
    {
        return count();
    }
    size_t capacity() const
    {
        return _capacity;
    }

    bool isEmpty()
    {
        return _putIndex == _getIndex;
    }
    bool isFull()
    {
        return _puIndext == _getIndex - 1 || (_getIndex == 0 && _putIndex == _capacity);
    }

private:
    size_t _capacity;
    T* _pBuffer;
    size_t _putIndex;
    size_t _getIndex;

private:
    RingBuffer();
    RingBuffer(const RingBuffer&);
    RingBuffer& operator=(const RingBuffer&);

}; // class RingBuffer

inline template <typename T>
bool RingBuffer<T>::push(const T& t)
{
    if (isFull())
        return false;

    _pBuffer[_putIndex] = t;

    if (_putIndex == _capacity)
        _putIndex = 0;
    else
        _putIndex++;

    return true;
}

inline template <typename T>
bool RingBuffer<T>::pop(T& t)
{
    if (isEmpty())
        return false;

    t = _pBuffer[_getIndex];

    if (_getIndex == _capacity)
        _getIndex = 0;
    else
        _getIndex++;

    return true;
}

} // namespace turbo

#endif // __TURBO_RING_BUFFER_H__
