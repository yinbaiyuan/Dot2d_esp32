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
#include "dtRenderer.h"
#include "../dtDirector.h"

NS_DT_BEGIN

void Renderer::initDotCanvas(uint16_t width,uint16_t height)
{
    _canvasSize.width = width;
    _canvasSize.height = height;
    _dotNums = width * height;
    if(_dotCanvas)
    {
        delete _dotCanvas;
    }
    _dotCanvas = (DTRGB *)malloc(sizeof(DTRGB) * _dotNums);
    memset(_dotCanvas, 0, sizeof(DTRGB) * _dotNums);
    if (_matrixIndex)
    {
        delete _matrixIndex;
    }
    _matrixIndex = (uint16_t* )malloc(sizeof(uint16_t) * _dotNums);
    for (int32_t i = 0; i < _dotNums; i++)
    {
        _matrixIndex[i] = i;
    }
}

DTRGB* Renderer::getDotCanvas()
{
    return _dotCanvas;
}

DTRGB* Renderer::getDotCanvas(uint16_t* width,uint16_t* height)
{
    *width = _canvasSize.width;
    *height = _canvasSize.height;
    return _dotCanvas;
}

void Renderer::clearDotCanvas(const DTRGB& color)
{
    for (uint16_t i = 0; i < _dotNums; i++)
    {
        _dotCanvas[i] = DTRGB(color.r,color.g,color.b);
    }
}

const Size& Renderer::getCanvasSize() const
{
    return _canvasSize;
}

uint16_t Renderer::getDotCount()
{
    return _canvasSize.width * _canvasSize.height;
}

void Renderer::setMatrixOrder(uint16_t x, uint16_t y,uint16_t order)
{
    if(x >= _canvasSize.width || y >= _canvasSize.height || order >= _canvasSize.width * _canvasSize.height)
    {
        return;
    }
    _matrixIndex[y * _canvasSize.width + x] = order;
}

uint16_t Renderer::XY(uint16_t x, uint16_t y)
{
    return _matrixIndex[y * _canvasSize.width + x];
}

Renderer::Renderer()
: _canvasSize(0,0)
{

}

Renderer::~Renderer()
{
    if(_dotCanvas)
    {
        delete _dotCanvas;
        _dotCanvas = nullptr;
    }
    if(_matrixIndex)
    {
        delete _matrixIndex;
        _matrixIndex = nullptr;
    }
}

void Renderer::drawDot(const Transform& transform,int32_t x,int32_t y,const DTRGB& c)
{
    //矩阵变换
    transform.transform(&x,&y);
    if(x>=0 && x<_canvasSize.width && y>=0 && y<_canvasSize.height)
    {
        _dotCanvas[XY(x,y)] = c;
    }
}

void Renderer::clear()
{
    this->clearDotCanvas(DTRGB(0,0,0));
}

void Renderer::render()
{
    Director::getInstance()->render();
}




NS_DT_END
