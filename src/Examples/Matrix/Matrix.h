/****************************************************************************
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

#ifndef __Matrix_H__
#define __Matrix_H__

#include "Dot2D/dot2d.h"

NS_DT_BEGIN

class Matrix : public Scene
{

protected:

    bool init() override;

public:

    STATIC_CREATE(Matrix);

};

class MatrixLayer : public Layer
{
protected:
    
    struct Animate
    {
        int16_t offset;
        float interval;
        float dInterval;
    };

    float showInterval = 0.0f;
    
    Animate* animateList = nullptr;
    
    uint8_t animateCount = 0;

public:

    virtual ~MatrixLayer();

protected:
    
    CanvasSprite* canvasSprite = nullptr;

public:

    STATIC_CREATE(MatrixLayer);

    virtual bool initLayer();
    
    void update(float dt) override;

};


NS_DT_END

#endif //__Matrix_H__