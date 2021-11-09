/****************************************************************************
Copyright (c) 2009      Sony Computer Entertainment Inc.
Copyright (c) 2021      Yinbaiyuan

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

#ifndef __DT_TRANSFORM_H__
#define __DT_TRANSFORM_H__

#include "../base/dtMacros.h"
#include "Vec2.h"

NS_DT_BEGIN

/**
 * @addtogroup base
 * @{
 */

class Transform
{
protected:

    Vec2 _translation;

    // float rotate;

    // float scale;

public:

    Transform(const Vec2& translation);

    Transform(const Transform& transform);
    
    ~Transform();

    void setTranslation(const Vec2& translation);

    Transform transform(const Transform& parentTransform) const;

    void transform(int32_t *x,int32_t *y) const;

    static const Transform IDENTITY;
};




/**
 end of base group
 @}
 */

NS_DT_END

#endif //__DT_TRANSFORM_H__