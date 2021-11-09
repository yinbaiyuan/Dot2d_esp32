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
#ifndef __DGF_DECODER_H__
#define __DGF_DECODER_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define DGF_GRAPH_FRAME_HEAD    0b00101100

struct DgfFrameInfo
{
    uint16_t duration;
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
    uint8_t transparenceType;
    uint8_t transparenceColorIndex;
    uint8_t disposalMode;
    uint8_t disposalParameter;
    int8_t translateX;
    int8_t translateY;
    uint8_t* data = nullptr;
};

struct DgfRGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    inline DgfRGB() __attribute__((always_inline))
    :r(0),g(0),b(0)
    {

    }

    inline DgfRGB( uint32_t colorcode)  __attribute__((always_inline))
    : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
    {

    }
    
    inline uint32_t getColor () const
    {
        uint32_t color = 0;
        color = (color + r)<<8;
        color = (color + g)<<8;
        color = (color + b);
        return color;
    }

    inline DgfRGB& operator= (const uint32_t colorcode) __attribute__((always_inline))
    {
        r = (colorcode >> 16) & 0xFF;
        g = (colorcode >>  8) & 0xFF;
        b = (colorcode >>  0) & 0xFF;
        return *this;
    }
};

struct DgfFrameInfoChain
{
    DgfFrameInfo info;
    DgfFrameInfoChain* next = nullptr;
};

struct DgfRGBChian
{
    DgfRGB color;
    DgfRGBChian* next = nullptr;
};

class DgfEncoder
{
private:
    
    const uint8_t EncoderVersion = 0b00010001;
    
protected:

    uint8_t _version = 0;

    uint8_t _width = 0;

    uint8_t _height = 0;
    
    uint8_t _colorType = 0;

    uint8_t _colorCount = 0;

    DgfRGBChian* _colorChain = nullptr;

    DgfFrameInfoChain* _frameChain = nullptr;

private:

    void freeEncoder();
    
    bool setFrameToBuffer(DgfFrameInfoChain* chain,uint8_t* buffer,uint32_t* dataPointer);
    
protected:
    
    void freeFrameInfoChian(DgfFrameInfoChain* chain);
    
    void freeDgfRGBChian(DgfRGBChian* chain);
    
    
    
    DgfFrameInfoChain* getLastFrameInfoChian();
    
    DgfRGBChian* getLastDgfRGBChian();
    
    uint32_t getFrameChainLenth(DgfFrameInfoChain* chain);
    
    uint32_t computeBufferLength();
    
    int8_t getColorIndexType();
    
public:

    DgfEncoder();

    virtual ~DgfEncoder();
    
    inline __attribute__((always_inline)) void set_bit(uint8_t* b,uint8_t w)
    {
        (*b)|=(1<<w);
    }
    
    inline __attribute__((always_inline)) void clr_bit(uint8_t* b,uint8_t w)
    {
        (*b)&=~(1<<w);
    }

    inline __attribute__((always_inline)) void set_bit(uint8_t* b,uint8_t w,bool v)
    {
        if (v){set_bit(b,w);}
        else {clr_bit(b,w);}
    }

    void setSize(uint8_t width,uint8_t height);

    bool addColor(uint8_t r,uint8_t g,uint8_t b);

    bool addFrame(uint16_t duration,                //持续时间
                  uint8_t x,                        //需要修改的图片数据区域的左上点X坐标
                  uint8_t y,                        //需要修改的图片数据区域的左上点Y坐标
                  uint8_t width,                    //需要修改的图片数据区域的宽度
                  uint8_t height,                   //需要修改的图片数据区域的高度
                  uint8_t transparenceType,         //当前帧透明像素处理方式
                  uint8_t transparenceColorIndex,   //当前透明像素索引值
                  uint8_t disposalMode,             //上一帧数据渲染处置方式
                  uint8_t disposalParameter,        //处置方式参数
                  int8_t translateX,                //上一帧X轴平移值
                  int8_t translateY,                //上一帧Y轴平移值
                  uint8_t* bitmap);                 //当前帧修改区域数据

    DgfFrameInfoChain* getFirstFrameInfoChain(){return  _frameChain;}
    
    uint8_t* encode(uint32_t* size);

};

class DgfDecoder : public DgfEncoder
{
private:

    DgfRGB* _colorTable = nullptr;
    
    uint8_t* _screenBuffer = nullptr;
    
    DgfFrameInfoChain* _frameChainPointer = nullptr;
    
    uint32_t _framesCount = 0;

private:

    void freeDecoder();

    bool getFrameFromBuffer(DgfFrameInfoChain* chain,const uint8_t* bitmap,uint32_t* dataPointer,uint32_t totalSize);
    
    void renderFrame();

public:

    DgfDecoder();

    virtual ~DgfDecoder();

    uint8_t width() const {return _width;}

    uint8_t height() const {return _height;}

    uint8_t colorType() const {return _colorType;}

    uint8_t colorCount() const {return _colorCount;}
    
    uint8_t getColorIndex(uint8_t x,uint8_t y) const;

    uint32_t getColor(uint8_t x,uint8_t y) const;

    void setColor(uint8_t index,uint8_t r,uint8_t g,uint8_t b);
    
    uint32_t getColorByIndex(uint8_t index) const;
    
    uint32_t getFramesCount() const {return _framesCount;}
    
    bool setNextFrame(bool loop);
    
    bool renderNextFrame(bool loop);
    
    uint16_t getDuration() const;

    bool decode(const uint8_t* bitmap,uint32_t size);
    
    const DgfFrameInfo* getFrameInfo(){return &(_frameChainPointer->info);}
};

#endif //__DGF_DECODER_H__
