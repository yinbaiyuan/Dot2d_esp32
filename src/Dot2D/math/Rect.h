/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org
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
#ifndef __RECT_H__
#define __RECT_H__

#include "../base/dtMacros.h"
#include "Vec2.h"
#include "Size.h"

NS_DT_BEGIN

/**
 * @addtogroup base
 * @{
 */

class Rect
{
public:

    Vec2 origin;

    Size size;

public:

    Rect();

    Rect(int32_t x, int32_t y, int32_t width, int32_t height);

    Rect(const Vec2& v, const Size& s);

    Rect(const Rect& r);

    void set(int32_t x, int32_t y, int32_t width, int32_t height);

    bool equals(const Rect& r) const;
    
    Rect& operator= (const Rect& r);

    bool operator==(const Rect& r) const;

    bool operator!=(const Rect& r) const;

    float getMinX() const;

    float getMidX() const;

    float getMaxX() const;

    float getMinY() const;

    float getMidY() const;

    float getMaxY() const;

    bool containsPoint(const Vec2& point) const;

    bool intersectsRect(const Rect& rect) const;

    bool intersectsCircle(const Vec2& center, float radius) const;

    Rect unionWithRect(const Rect & rect) const;
    
    void merge(const Rect& rect);

public:
    
    static const Rect ZERO;

};

/**
 end of base group
 @}
 */

NS_DT_END

#endif //__RECT_H__