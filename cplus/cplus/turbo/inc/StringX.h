// -------------------------------------------------------------------------------------
// StringX.h
//
// StringX class
//
// -------------------------------------------------------------------------------------
#ifndef __TURBO_STRING_X_H__
#define __TURBO_STRING_X_H__

#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

namespace turbo {

class StringX {

public:
    StringX(const char* s, size_t len = 0)
        : _stringBuffer(_newStringBuffer(s, len > 0 ? len : s == 0 ? 0 : strlen(s)))
    {
    }

    StringX()
        : _stringBuffer(_newStringBuffer("", 1))
    {
    }

    StringX(const StringX& string)
        : _stringBuffer(_newStringBuffer(string.get(), string.length()))
    {
    }

    StringX(const int from_int)
    {
        const int _INT_SIZE = 12; // sign + sizeof(MAX_INT 0x7fffffff=2147483647)=11
        char buf[_INT_SIZE] = { 0 };
        sprintf(buf, "%d", from_int);
        _stringBuffer = _newStringBuffer(buf, _INT_SIZE);
    }

    StringX(const float from_float)
    {
        const int _FLOAT_SIZE = 12 + 3; // sign + sizeof(MAX_INT 0x7fffffff=2147483647)=11
        char buf[_FLOAT_SIZE] = { 0 };
        sprintf(buf, "%.2f", from_float);
        _stringBuffer = _newStringBuffer(buf, _FLOAT_SIZE);
    }

    ~StringX()
    {
        _deleteStringBuffer();
    }

    StringX& operator=(const char* s)
    {
        _deleteStringBuffer();
        _stringBuffer = _newStringBuffer(s, s == 0 ? 0 : strlen(s));
        return *this;
    }

    StringX& operator=(const StringX& string)
    {
        _deleteStringBuffer();
        _stringBuffer = _newStringBuffer(string.get(), string.length());
        return *this;
    }

    StringX operator+(const char* s)
    {
        char* newBuffer = _newStringBuffer(get(), length(), s, s == 0 ? 0 : strlen(s));
        StringX tmp(newBuffer);
        if (newBuffer != 0)
            delete[] newBuffer;
        return tmp;
    }

    StringX operator+(const StringX& string)
    {
        char* newBuffer = _newStringBuffer(get(), length(), string.get(), string.length());
        StringX tmp(newBuffer);
        if (newBuffer != 0)
            delete[] newBuffer;
        return tmp;
    }

    StringX& operator+=(const char* s)
    {
        char* newBuffer = _newStringBuffer(get(), length(), s, s == 0 ? 0 : strlen(s));
        _deleteStringBuffer();
        _stringBuffer = newBuffer;
        return *this;
    }

    StringX& operator+=(const StringX& string)
    {
        char* newBuffer = _newStringBuffer(get(), length(), string.get(), string.length());
        _deleteStringBuffer();
        _stringBuffer = newBuffer;
        return *this;
    }

    int compare(const char* s)
    {
        return strcmp(get(), s);
    }
    int compare(const StringX& string)
    {
        return strcmp(get(), string.get());
    }
    int compareCaseInsensitive(const char* s)
    {
        return strcasecmp(get(), s);
    }
    int compareCaseInsensitive(const StringX& another)
    {
        return strcasecmp(get(), another.get());
    }

    bool operator==(const char* s)
    {
        return compare(s) == 0;
    }
    bool operator==(const StringX& another)
    {
        return compare(another) == 0;
    }
    bool operator!=(const char* s)
    {
        return !operator==(s);
    }
    bool operator!=(const StringX& another)
    {
        return !operator==(another);
    }

    size_t length() const
    {
        return _stringBuffer == 0 ? 0 : strlen(_stringBuffer);
    }
    const char* get() const
    {
        return static_cast<const char*>(_stringBuffer);
    }
    operator const char*()
    {
        return static_cast<const char*>(_stringBuffer);
    }

    bool isEmpty() const
    {
        return _stringBuffer != 0 && _stringBuffer[0] == 0;
    }

    enum TrimOption {
        kTrimLeft = 1,
        kTrimRight = 2,
        kTrimAll = kTrimLeft | kTrimRight
    };

    StringX& trim(TrimOption option = kTrimAll);

    StringX subString(size_t start, size_t length);
    char operator[](size_t index) const
    {
        return _stringBuffer[index];
    }
    char& operator[](size_t index)
    {
        return _stringBuffer[index];
    }

    int find(char c, size_t start = 0)
    {
        char* pChar = strchr(_stringBuffer + start, c);
        return pChar == 0 ? -1 : pChar - _stringBuffer;
    }

    int find(const char* s, size_t start = 0)
    {
        char* pSubStringX = strstr(_stringBuffer + start, s);
        return pSubStringX == 0 ? -1 : pSubStringX - _stringBuffer;
    }

    int findCaseInsensitive(const char* s, size_t start = 0)
    {
        char* pSubStringX = strcasestr(_stringBuffer + start, s);
        return pSubStringX == 0 ? -1 : pSubStringX - _stringBuffer;
    }
    int find(const StringX& string, size_t start = 0)
    {
        return find(string.get(), start);
    }
    int findCaseInsensitive(const StringX& string, size_t start = 0)
    {
        return findCaseInsensitive(string.get(), start);
    }

    bool startsWith(const char* s, size_t len = 0)
    {
        return strncmp(_stringBuffer, s, len > 0 ? len : s == 0 ? 0 : strlen(s));
    }
    bool startsWith(const StringX& prefix)
    {
        return startsWith(prefix.get());
    }
    bool startsWithCaseInsensitive(const char* s, size_t len = 0)
    {
        return strncasecmp(_stringBuffer, s, len > 0 ? len : s == 0 ? 0 : strlen(s));
    }
    bool startsWithCaseInsensitive(const StringX& prefix)
    {
        return startsWithCaseInsensitive(prefix.get());
    }

    bool endsWith(const char* s);
    bool endsWith(const StringX& suffix)
    {
        return endsWith(suffix.get());
    }

private:
    char* _stringBuffer;

private:
    static char* _newStringBuffer(const char* s, size_t len);
    static char* _newStringBuffer(const char* s1, size_t len1, const char* s2, size_t len2);
    void _deleteStringBuffer();

}; // class StringX

} // namespace turbo

#endif // __TURBO_STRING_H__
