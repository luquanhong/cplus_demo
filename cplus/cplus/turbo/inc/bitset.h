// -----------------------------------------------------------------------------
// bitset.h
//
// A simple wrapper for bitmask data
//
// -----------------------------------------------------------------------------
#ifndef __TURBO_BITSET_H__
#define __TURBO_BITSET_H__

#include <stdint.h>

namespace turbo {

class bitset {

public:
    bitset()
        : _data(0)
    {
    }
    bitset(uint32_t data)
        : _data(data)
    {
    }
    bitset(const bitset& another)
        : _data(another._data)
    {
    }

    bitset& operator=(const bitset& another)
    {
        _data = another._data;
        return *this;
    }
    bitset& operator=(uint32_t data)
    {
        _data = data;
        return *this;
    }

    bool operator==(const bitset& another)
    {
        return _data == another._data;
    }
    bool operator==(uint32_t data)
    {
        return _data == data;
    }

    bool operator!=(const bitset& another)
    {
        return _data != another._data;
    }
    bool operator!=(uint32_t data)
    {
        return _data != data;
    }

    bitset& set(uint32_t bitmask)
    {
        _data |= bitmask;
        return *this;
    }
    bitset& unset(uint32_t bitmask)
    {
        _data &= ~bitmask;
        return *this;
    }
    bool is_set(uint32_t bitmask)
    {
        return (_data & bitmask) == bitmask;
    }

private:
    uint32_t _data;

}; // class bitset

} // namespace turbo

#endif // __TURBO_BITSET_H__
