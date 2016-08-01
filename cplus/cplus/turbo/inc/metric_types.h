// ------------------------------------------------------------------------------
// metric_types.h
//
// The file defined types for metrics
//
// ------------------------------------------------------------------------------
#ifndef __TURBO_METRIC_TYPES_H__
#define __TURBO_METRIC_TYPES_H__

#include <sys/types.h>

namespace turbo {

struct Point {

public:
    Point(int x, int y)
        : _x(x)
        , _y(y)
    {
    }
    Point()
        : _x(0)
        , _y(0)
    {
    }

    int getX() const
    {
        return _x;
    }
    int getY() const
    {
        return _y;
    }

    Point(const Point& another)
        : _x(another._x)
        , _y(another._y)
    {
    }

    Point& operator=(const Point& another)
    {
        _x = another._x, _y = another._y;
        return *this;
    }

private:
    int _x;
    int _y;
};

struct Size {

public:
    Size()
        : _width(0)
        , _height(0)
    {
    }
    Size(size_t width, size_t height)
        : _width(width)
        , _height(height)
    {
    }
    Size(const Size& another)
        : _width(another._width)
        , _height(another._height)
    {
    }

    Size& operator=(const Size& another)
    {
        _width = another._width, _height = another._height;
        return *this;
    }

    size_t getWidth() const
    {
        return _width;
    }
    size_t getHeight() const
    {
        return _height;
    }

private:
    size_t _width;
    size_t _height;
};

struct Rect {

public:
    Rect()
        : _origin()
        , _size()
    {
    }
    Rect(const Point& origin, const Size& size)
        : _origin(origin)
        , _size(size)
    {
    }
    Rect(int x, int y, size_t width, size_t height)
        : _origin(x, y)
        , _size(width, height)
    {
    }

    Rect(const Rect& another)
        : _origin(another._origin)
        , _size(another._size)
    {
    }

    Rect& operator=(const Rect& another)
    {
        _origin = another._origin, _size = another._size;
        return *this;
    }

    const Point& getOrigin() const
    {
        return _origin;
    }
    const Size& getSize() const
    {
        return _size;
    }

    int getOriginX()
    {
        return _origin.getX();
    }
    int getOriginY()
    {
        return _origin.getY();
    }
    size_t getWidth()
    {
        return _size.getWidth();
    }
    size_t getHeight()
    {
        return _size.getHeight();
    }

    Point getTopLeft() const
    {
        return _origin;
    }
    Point getTopRight() const
    {
        return Point(_origin.getX() + _size.getWidth(), _origin.getY());
    }
    Point getBottomLeft() const
    {
        return Point(_origin.getX(), _origin.getY() + _size.getHeight());
    }
    Point getBottomRight() const
    {
        return Point(_origin.getX() + _size.getWidth(), _origin.getY() + _size.getHeight());
    }

    bool isEmpty()
    {
        return _size.getWidth() == 0 || _size.getHeight() == 0;
    }

private:
    Point _origin;
    Size _size;
};

} // namespace turbo

#endif // __TURBO_METRIC_TYPES_H__
