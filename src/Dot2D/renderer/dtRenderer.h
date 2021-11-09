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
#ifndef __DT_RENDERER_H_
#define __DT_RENDERER_H_

#include <vector>

#include "../base/dtMacros.h"
#include "../base/dtTypes.h"
#include "../math/dtGeometry.h"
#include "../math/dtTransform.h"

/**
 * @addtogroup renderer
 * @{
 */

NS_DT_BEGIN

class Renderer
{
private:

    DTRGB* _dotCanvas = nullptr;
    
    Size _canvasSize;

    uint16_t _dotNums = 0;

    uint16_t* _matrixIndex = nullptr;

protected:

    void initDotCanvas(uint16_t width,uint16_t height);

    DTRGB* getDotCanvas();

    DTRGB* getDotCanvas(uint16_t* width,uint16_t* height);

    void clearDotCanvas(const DTRGB& color);

    const Size& getCanvasSize() const;

    uint16_t getDotCount();

    uint16_t XY(uint16_t x, uint16_t y);

    friend class Director;

public:

    Renderer();

    ~Renderer();

    void setMatrixOrder(uint16_t x, uint16_t y,uint16_t order);

    void drawDot(const Transform& transform,int32_t x,int32_t y,const DTRGB& c);

    void clear();

    void render();

    

};

NS_DT_END

/**
 end of support group
 @}
 */

#endif //__DT_RENDERER_H_