/****************************************************************************
Copyright (c) 2013      Adafruit Industries.
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

#ifndef __DT_SPRITE_CANVAS_H__
#define __DT_SPRITE_CANVAS_H__

#include "Print.h"
#include <string>
#include "base/dtMacros.h"
#include "base/dtRef.h"
#include "base/dtTypes.h"
#include "math/dtGeometry.h"
#include "third/gfxfont.h"
NS_DT_BEGIN

class SpriteCanvas : public Ref , public Print
{
private:

    DTRGB* _canvasBuffer = nullptr;
    
protected:
    
    uint16_t textHSpace = 0;
    uint16_t textVSpace = 0;

    uint16_t textWidth = 0;
    uint16_t textHeight = 0;
    
    uint16_t WIDTH;
    uint16_t HEIGHT;
    
    uint16_t _width;
    uint16_t _height;
    
    int16_t cursor_x;
    int16_t cursor_y;
    
    uint8_t textSize;
    
    uint8_t rotation;
    
    bool wrap;
    
    GFXfont *gfxFont;
    
    DTRGB textColor;
    
    DTRGB textBgColor;


protected:

    bool init(uint16_t w, uint16_t h);
    
    void setBuffer(uint16_t w, uint16_t h);
    
    static void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy,uint16_t width,uint16_t height,uint8_t textSize,bool wrap,const GFXfont *gfxFont);

    static void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h,uint16_t width,uint16_t height,uint8_t textSize,bool wrap,const GFXfont *gfxFont);
public:

    SpriteCanvas(uint16_t w, uint16_t h);

    ~SpriteCanvas();

    static SpriteCanvas* create();

    static SpriteCanvas* create(Size size);

    static SpriteCanvas* create(uint16_t w, uint16_t h);

    static SpriteCanvas* create(uint16_t w, uint16_t h,const std::string& s,const GFXfont *f = (const GFXfont *)__null,uint8_t tSize = 1);

    static SpriteCanvas* create(const std::string& s,const GFXfont *f = (const GFXfont *)__null,uint8_t tSize = 1);

    uint16_t getTextWidth(){return textWidth;}

    void setTextHSpace(uint16_t s){textHSpace = s;}

    void setTextVSpace(uint16_t s){textVSpace = s;}

    DTRGB colorWithXY(uint16_t x,uint16_t y) const;

    bool posInRect(uint16_t x,uint16_t y) const;

    void drawChar(int16_t x, int16_t y, unsigned char c, DTRGB& color,DTRGB& bg, uint8_t size);

    void canvasReset();

    DTRGB getTextcolor(){return textColor;}

    DTRGB* getBuffer(){return _canvasBuffer;}

    void copyBuffer(DTRGB* buffer,uint32_t length);
    
public:
    
    virtual void drawPixel(int16_t x, int16_t y,const DTRGB& color);
    
    virtual size_t write(uint8_t);
    
    virtual void startWrite(void);
    virtual void writePixel(int16_t x, int16_t y,const DTRGB& color);
    virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,const DTRGB& color);
    virtual void writeFastVLine(int16_t x, int16_t y, int16_t h,const DTRGB& color);
    virtual void writeFastHLine(int16_t x, int16_t y, int16_t w,const DTRGB& color);
    virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,const DTRGB& color);
    virtual void writeGradientLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,const DTHSV& colorFrom,const DTHSV& colorTo);
    
    virtual void endWrite(void);
    
    virtual void setRotation(uint8_t r);
    virtual void invertDisplay(bool i);
    
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, const DTRGB& color);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, const DTRGB& color);
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, const DTRGB& color);
    virtual void fillScreen(const DTRGB& color);
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const DTRGB& color);
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, const DTRGB& color);
    
    void drawCircle(int16_t x0, int16_t y0, int16_t r, const DTRGB& color);
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, const DTRGB& color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, const DTRGB& color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, const DTRGB& color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, const DTRGB& color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, const DTRGB& color);
    void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, const DTRGB& color);
    void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, const DTRGB& color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, const DTRGB& color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, const DTRGB& color, const DTRGB& bg);
    void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, const DTRGB& color);
    void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, const DTRGB& color, const DTRGB& bg);
    void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, const DTRGB& color);
    void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
    void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
    void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
    void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
    void drawRGBBitmap(int16_t x, int16_t y, const DTRGB bitmap[], int16_t w, int16_t h);
    void drawRGBBitmap(int16_t x, int16_t y, DTRGB *bitmap, int16_t w, int16_t h);
    void drawRGBBitmap(int16_t x, int16_t y, const DTRGB bitmap[], const uint8_t mask[], int16_t w, int16_t h);
    void drawRGBBitmap(int16_t x, int16_t y, DTRGB *bitmap, uint8_t *mask, int16_t w, int16_t h);
    void drawChar(int16_t x, int16_t y, unsigned char c, const DTRGB& color, const DTRGB& bg, uint8_t size);
    void setCursor(int16_t x, int16_t y);
    void setTextColor(const DTRGB& c);
    void setTextColor(const DTRGB& c, const DTRGB& bg);
    void setTextSize(uint8_t s);
    void setTextWrap(bool w);
    void setFont(const GFXfont *f = NULL);
    

    
    
    uint16_t height(void) const;
    uint16_t width(void) const;

    uint8_t getRotation(void) const;

    int16_t getCursorX(void) const;
    int16_t getCursorY(void) const;
    

};

NS_DT_END


#endif //__DT_SPRITE_CANVAS_H__
