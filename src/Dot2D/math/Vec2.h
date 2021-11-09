/****************************************************************************
Copyright (c) 2013 BlackBerry Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
Copyright (c) 2021      Yinbaiyuan

http://www.cocos2d-x.org
https://www.yinbaiyuan.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __VEC2_H__
#define __VEC2_H__

#include "../base/dtMacros.h"

NS_DT_BEGIN

/**
 * @addtogroup base
 * @{
 */

class Vec2
{
public:
    
    int32_t x;

    int32_t y;

public:

    INLINE Vec2() 
    :x(0),y(0)
    {

    };

    INLINE Vec2(int32_t xx, int32_t yy)
    :x(xx),y(yy)
    {

    };

    INLINE void set(const Vec2& v)
    {
        this->x = v.x;
        this->y = v.y;
    };

    INLINE void set(int32_t xx, int32_t yy)
    {
        this->x = xx;
        this->y = yy;
    };

    INLINE bool isZero() const
    {
        return x == 0 && y == 0;
    };

    INLINE bool isOne() const
    {
        return x == 1 && y == 1;
    };

    INLINE void add(const Vec2& v)
    {
        x += v.x;
        y += v.y;
    };

    static void add(const Vec2& v1, const Vec2& v2, Vec2* dst);

    INLINE void subtract(const Vec2& v)
    {
        x -= v.x;
        y -= v.y;
    };

    static void subtract(const Vec2& v1, const Vec2& v2, Vec2* dst);

    void clamp(const Vec2& min, const Vec2& max);

    static void clamp(const Vec2& v, const Vec2& min, const Vec2& max, Vec2* dst);

    INLINE int32_t dot(const Vec2& v) const
    {
        return (x * v.x + y * v.y);
    };

    static int32_t dot(const Vec2& v1, const Vec2& v2);

    INLINE void negate()
    {
        x = -x;
        y = -y;
    };

    void normalize();

    Vec2 getNormalized() const;

    INLINE void scale(float scalar)
    {
        x *= scalar;
        y *= scalar;
    };

    INLINE void scale(float scalarX,float scalarY)
    {
        x *= scalarX;
        y *= scalarY;
    };

    void rotate(const Vec2& point, float angle);

    INLINE void setZero()
    {
        x = y = 0;
    };

    INLINE bool equals(const Vec2& v) const
    {
        return *this == v;
    };

    INLINE Vec2 operator+(const Vec2& v) const
    {
        Vec2 result(*this);
        result.add(v);
        return result;
    };

    INLINE Vec2& operator+=(const Vec2& v)
    {
        add(v);
        return *this;
    };

    INLINE Vec2 operator-(const Vec2& v) const
    {
        Vec2 result(*this);
        result.subtract(v);
        return result;
    };

    INLINE Vec2& operator-=(const Vec2& v)
    {
        subtract(v);
        return *this;
    };

    INLINE Vec2 operator-() const
    {
        Vec2 result(*this);
        result.negate();
        return result;
    };

    INLINE Vec2 operator*(float a) const
    {
        Vec2 result(*this);
        result.scale(a);
        return result;
    };

    INLINE Vec2& operator*=(float a)
    {
        scale(a);
        return *this;
    };

    INLINE Vec2 operator/(float a) const
    {
        return Vec2(this->x / a, this->y / a);
    };

    INLINE bool operator<(const Vec2& v) const
    {
        if (x == v.x)
        {
            return y < v.y;
        }
        return x < v.x;
    };

    INLINE bool operator>(const Vec2& v) const
    {
        if (x == v.x)
        {
            return y > v.y;
        }
        return x > v.x;
    };

    INLINE bool operator==(const Vec2& v) const
    {
        return x==v.x && y==v.y;
    };

    INLINE bool operator!=(const Vec2& v) const
    {
        return x!=v.x || y!=v.y;
    };

};

/**
 end of base group
 @}
 */

NS_DT_END

#endif //__VEC2_H__