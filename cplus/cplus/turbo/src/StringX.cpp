// -----------------------------------------------------------------------
// StringX.cpp
//
// -----------------------------------------------------------------------

#include <ctype.h>

#include <StringX.h>

using namespace turbo;

// If len == 0, return an empty buffer (not NULL!)
char* StringX::_newStringBuffer(const char* s, size_t len)
{
    char* buffer;

    buffer = new char[len + 1];
    if (s != 0)
        strncpy(buffer, s, len);
    buffer[len] = 0;

    return buffer;
}

// Empty buffer if len1 == 0 && len2 == 0
char* StringX::_newStringBuffer(const char* s1, size_t len1, const char* s2, size_t len2)
{
    char* buffer;

    buffer = new char[len1 + len2 + 1];
    if (s1 != 0)
        strncpy(buffer, s1, len1);
    if (s2 != 0)
        strncpy(buffer + len1, s2, len2);
    buffer[len1 + len2] = 0;

    return buffer;
}

void StringX::_deleteStringBuffer()
{
    if (_stringBuffer != 0) {
        delete[] _stringBuffer;
        _stringBuffer = 0;
    }
}

StringX& StringX::trim(StringX::TrimOption option /* = kTrimAll */)
{
    if (_stringBuffer == 0 || _stringBuffer[0] == 0)
        return *this;

    bool trimmed = false;

    char* pStart = _stringBuffer;

    if (option & kTrimLeft) {

        while (*pStart != 0 && isspace(*pStart)) {
            trimmed = true, pStart++;
        }

        if (*pStart == 0) {
            _deleteStringBuffer();
            _stringBuffer = _newStringBuffer("", 0);
            return *this;
        }

    } // kTrimLeft

    char* pEnd = _stringBuffer + strlen(_stringBuffer) - 1;

    if (option & kTrimRight) {
        while (pEnd > pStart && isspace(*pEnd)) {
            trimmed = true, pEnd--;
        }
    }

    if (trimmed) {
        _deleteStringBuffer();
        _stringBuffer = _newStringBuffer(pStart, pEnd - pStart + 1);
    }

    return *this;
}

StringX StringX::subString(size_t start, size_t length)
{
    if (_stringBuffer == 0)
        return StringX();

    size_t len = strlen(_stringBuffer);
    if (start > len)
        start = len;

    if (start + length > len)
        length = len - start;

    return StringX(_stringBuffer + start, length);
}

bool StringX::endsWith(const char* s)
{
    if (s == 0 || s[0] == 0)
        return false;

    const char* pStringX = s + strlen(s) - 1;
    char* pStringBuffer = _stringBuffer + strlen(_stringBuffer) - 1;

    while (pStringX > s && pStringBuffer > _stringBuffer) {
        if (*pStringX != *pStringBuffer)
            return false;
    }

    return pStringX == s;
}

#ifdef TEST_STRINGX

#include <stdio.h>

int main()
{
    StringX s1("This is");
    StringX s2(s1);
    StringX s3;

    s3 = " a string";

    printf("%s, %s, %s\n", s1.get(), s2.get(), s3.get());

    s2 = s1 + s3;

    printf("%s, %s, %s\n", s1.get(), s2.get(), s3.get());

    s3 += s1;

    printf("%s, %s, %s\n", s1.get(), s2.get(), s3.get());

    s1[2] = 'x';

    printf("%s, %s, %s\n", s1.get(), s2.get(), s3.get());

    printf("s1[3] = %c\n", s1[3]);

    // compare
    printf("s1.compare(\"This is\") = %d, s2.compare(s1) = %d, s3.compare(\" a stringThis is\") = %d\n",
           s1.compare("This is"), s2.compare(s1), s3.compare(" a stringThis is"));

    printf("s1.get = %s, s1.length = %d\n", s1.get(), s1.length());

    printf("s3.trim = %s\n", s3.trim().get());

    return 0;
}

#endif
