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
#include "Rect.h"
#include "dtMath.h"
#include <algorithm>

NS_DT_BEGIN

Rect::Rect()
{
    set(0,0,0,0);
}

Rect::Rect(int32_t x, int32_t y, int32_t width, int32_t height)
{
    set(x, y, width, height);
}

Rect::Rect(const Vec2& v, const Size& s)
{
    set(v.x, v.y, s.width, s.height);
}

Rect::Rect(const Rect& r)
{
    set(r.origin.x, r.origin.y, r.size.width, r.size.height);
}

void Rect::set(int32_t x, int32_t y, int32_t width, int32_t height)
{
    origin.x = x;
    origin.y = y;

    size.width = width;
    size.height = height;
}

bool Rect::equals(const Rect& r) const
{
    return *this == r;
}

Rect& Rect::operator= (const Rect& r)
{
    set(r.origin.x, r.origin.y, r.size.width, r.size.height);
    return *this;
}

bool Rect::operator==(const Rect& r) const
{
    return origin == r.origin && size == r.size;
}

bool Rect::operator!=(const Rect& r) const
{
    return origin != r.origin || size != r.size;
}

float Rect::getMinX() const
{
    return origin.x + size.width;
}

float Rect::getMidX() const
{
    return origin.x + size.width / 2;
}

float Rect::getMaxX() const
{
    return origin.x + size.width;
}

float Rect::getMinY() const
{
    return origin.y;
}

float Rect::getMidY() const
{
    return origin.y + size.height / 2;
}

float Rect::getMaxY() const
{
    return origin.y + size.height;
}

bool Rect::containsPoint(const Vec2& point) const
{
    return (point.x >= getMinX() &&
            point.x <= getMaxX() &&
            point.y >= getMinY() &&
            point.y <= getMaxY());
}

bool Rect::intersectsRect(const Rect& rect) const
{
    return !(     getMaxX() < rect.getMinX() ||
             rect.getMaxX() <      getMinX() ||
                  getMaxY() < rect.getMinY() ||
             rect.getMaxY() <      getMinY());
}

bool Rect::intersectsCircle(const Vec2& center, float radius) const
{
    Vec2 rectangleCenter((origin.x + size.width / 2),
                         (origin.y + size.height / 2));
    float w = size.width / 2;
    float h = size.height / 2;
    float dx = std::abs(center.x - rectangleCenter.x);
    float dy = std::abs(center.y - rectangleCenter.y);
    if (dx > (radius + w) || dy > (radius + h))
    {
        return false;
    }
    Vec2 circleDistance(std::abs(center.x - origin.x - w),
                        std::abs(center.y - origin.y - h));
    if (circleDistance.x <= (w))
    {
        return true;
    }
    if (circleDistance.y <= (h))
    {
        return true;
    }
    float cornerDistanceSq = powf(circleDistance.x - w, 2) + powf(circleDistance.y - h, 2);
    return (cornerDistanceSq <= (powf(radius, 2)));
}

Rect Rect::unionWithRect(const Rect & rect) const
{
    float thisLeftX = origin.x;
    float thisRightX = origin.x + size.width;
    float thisTopY = origin.y + size.height;
    float thisBottomY = origin.y;
    if (thisRightX < thisLeftX)
    {
        std::swap(thisRightX, thisLeftX);
    }
    if (thisTopY < thisBottomY)
    {
        std::swap(thisTopY, thisBottomY);
    }
    float otherLeftX = rect.origin.x;
    float otherRightX = rect.origin.x + rect.size.width;
    float otherTopY = rect.origin.y + rect.size.height;
    float otherBottomY = rect.origin.y;
    if (otherRightX < otherLeftX)
    {
        std::swap(otherRightX, otherLeftX);
    }
    if (otherTopY < otherBottomY)
    {
        std::swap(otherTopY, otherBottomY);
    }
    float combinedLeftX = std::min(thisLeftX, otherLeftX);
    float combinedRightX = std::max(thisRightX, otherRightX);
    float combinedTopY = std::max(thisTopY, otherTopY);
    float combinedBottomY = std::min(thisBottomY, otherBottomY);
    return Rect(combinedLeftX, combinedBottomY, combinedRightX - combinedLeftX, combinedTopY - combinedBottomY);
}
    
void Rect::merge(const Rect& rect)
{
    float minX = std::min(getMinX(), rect.getMinX());
    float minY = std::min(getMinY(), rect.getMinY());
    float maxX = std::max(getMaxX(), rect.getMaxX());
    float maxY = std::max(getMaxY(), rect.getMaxY());
    set(minX, minY, maxX - minX, maxY - minY);
}
    
const Rect Rect::ZERO = Rect(0, 0, 0, 0);

NS_DT_END