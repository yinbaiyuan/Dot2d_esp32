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

#include "dtSpriteCanvas.h"
#include "Dot2D/third/glcdfont.h"
#include "dtDirector.h"

NS_DT_BEGIN

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

bool SpriteCanvas::init(uint16_t w, uint16_t h)
{
    this->setBuffer(w,h);
    return true;
}

void SpriteCanvas::setBuffer(uint16_t w, uint16_t h)
{
    if(_canvasBuffer)
    {
        DT_SAFE_DELETE(_canvasBuffer);
    }
    _canvasBuffer = (DTRGB*)malloc(w * h * sizeof(DTRGB));
    memset(_canvasBuffer , 0 , w * h * sizeof(DTRGB));
}

void SpriteCanvas::charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy,uint16_t width,uint16_t height,uint8_t textSize,bool wrap,const GFXfont *gfxFont)
{
    if(gfxFont) {
        
        if(c == '\n') { // Newline?
            *x  = 0;    // Reset x to zero, advance y by one line
            *y += textSize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        } else if(c != '\r') { // Not a carriage return; is normal char
            uint8_t first = pgm_read_byte(&gfxFont->first),
            last  = pgm_read_byte(&gfxFont->last);
            if((c >= first) && (c <= last)) { // Char present in this font?
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(
                                                                  &gfxFont->glyph))[c - first]);
                uint8_t gw = pgm_read_byte(&glyph->width),
                gh = pgm_read_byte(&glyph->height),
                xa = pgm_read_byte(&glyph->xAdvance);
                int8_t  xo = pgm_read_byte(&glyph->xOffset),
                yo = pgm_read_byte(&glyph->yOffset);
                if(wrap && ((*x+(((int16_t)xo+gw)*textSize)) > width)) {
                    *x  = 0; // Reset x to zero, advance y by one line
                    *y += textSize * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
                }
                int16_t ts = (int16_t)textSize,
                x1 = *x + xo * ts,
                y1 = *y + yo * ts,
                x2 = x1 + gw * ts - 1,
                y2 = y1 + gh * ts - 1;
                if(x1 < *minx) *minx = x1;
                if(y1 < *miny) *miny = y1;
                if(x2 > *maxx) *maxx = x2;
                if(y2 > *maxy) *maxy = y2;
                *x += xa * ts;
            }
        }
        
    } else { // Default font
        
        if(c == '\n') {                     // Newline?
            *x  = 0;                        // Reset x to zero,
            *y += textSize * 8;             // advance y one line
            // min/max x/y unchaged -- that waits for next 'normal' character
        } else if(c != '\r') {  // Normal char; ignore carriage returns
            if(wrap && ((*x + textSize * 6) > width)) { // Off right?
                *x  = 0;                    // Reset x to zero,
                *y += textSize * 8;         // advance y one line
            }
            int x2 = *x + textSize * 6 - 1, // Lower-right pixel of char
            y2 = *y + textSize * 8 - 1;
            if(x2 > *maxx) *maxx = x2;      // Track max x, y
            if(y2 > *maxy) *maxy = y2;
            if(*x < *minx) *minx = *x;      // Track min x, y
            if(*y < *miny) *miny = *y;
            *x += textSize * 6;             // Advance x one char
        }
    }
}

void SpriteCanvas::getTextBounds(const char *str, int16_t x, int16_t y,
                                 int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h,uint16_t width,uint16_t height,uint8_t textSize,bool wrap,const GFXfont *gfxFont) {
    uint8_t c; // Current character
    
    *x1 = x;
    *y1 = y;
    *w  = *h = 0;
    
    int16_t minx = width, miny = height, maxx = -1, maxy = -1;
    
    while((c = *str++))
        SpriteCanvas::charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy,width,height,textSize,wrap,gfxFont);
    
    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}


SpriteCanvas::SpriteCanvas(uint16_t w, uint16_t h)
:WIDTH(w), HEIGHT(h)
{
    _width      = WIDTH;
    _height     = HEIGHT;
    rotation    = 0;
    cursor_y    = 0;
    cursor_x    = 0;
    textSize    = 1;
    textColor   = DTRGB(255, 255, 255);
    textBgColor = DTRGB(255, 255, 255);;
    wrap        = true;
    gfxFont     = nullptr;
}

SpriteCanvas::~SpriteCanvas()
{
    DT_SAFE_DELETE(_canvasBuffer);
}

SpriteCanvas* SpriteCanvas::create()
{
    Size size = Director::getInstance()->getCanvasSize();
    return SpriteCanvas::create(size.width,size.height);
}

SpriteCanvas* SpriteCanvas::create(Size size)
{
    return SpriteCanvas::create(size.width,size.height);
}

SpriteCanvas* SpriteCanvas::create(uint16_t w, uint16_t h)
{
    SpriteCanvas *spriteCanvas = new SpriteCanvas(w,h);
    if(spriteCanvas && spriteCanvas->init(w,h))
    {
        spriteCanvas->autorelease();
        return spriteCanvas;
    }
    DT_SAFE_DELETE(spriteCanvas);
    return nullptr;
}

SpriteCanvas* SpriteCanvas::create(uint16_t w, uint16_t h,const std::string& s,const GFXfont *f,uint8_t tSize)
{
    
    SpriteCanvas *spriteCanvas = SpriteCanvas::create(w,h);
    if(spriteCanvas)
    {
        spriteCanvas->setTextWrap(false);
        spriteCanvas->setFont(f);
        spriteCanvas->setTextSize(tSize);
        spriteCanvas->canvasReset();
        spriteCanvas->print(s.c_str());
    }
    return spriteCanvas;
}

SpriteCanvas* SpriteCanvas::create(const std::string& s,const GFXfont *f,uint8_t tSize)
{
    uint16_t w=0;
    uint16_t h=0;
    int16_t x=0;
    int16_t y=0;
    int16_t x1=0;
    int16_t y1=0;
    SpriteCanvas::getTextBounds(s.c_str(),x,y,&x1,&y1,&w,&h,0,0,tSize,false,f);
    return SpriteCanvas::create(w,h,s,f,tSize);
}



DTRGB SpriteCanvas::colorWithXY(uint16_t x,uint16_t y) const
{
    return _canvasBuffer[x + y * WIDTH];
}

bool SpriteCanvas::posInRect(uint16_t x,uint16_t y) const
{
    if (x>=0 && x<width() && y>=0 && y<height())
    {
        return true;
    }
    return false;
}

void SpriteCanvas::drawChar(int16_t x, int16_t y, unsigned char c, DTRGB& color,DTRGB& bg, uint8_t size)
{
    if(!gfxFont) { // 'Classic' built-in font
        if((x >= _width)            || // Clip right
           (y >= _height)           || // Clip bottom
           ((x + 6 * size - 1) < 0) || // Clip left
           ((y + 8 * size - 1) < 0))   // Clip top
            return;
        
        if((c >= 176)) c++; // Handle 'classic' charset behavior
        
        startWrite();
        for(int8_t i=0; i<5; i++ ) { // Char bitmap = 5 columns
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            for(int8_t j=0; j<8; j++, line >>= 1) {
                if(line & 1) {
                    if(size == 1)
                        writePixel(x+i, y+j, color);
                    else
                        writeFillRect(x+i*size, y+j*size, size, size, color);
                } else if(bg != color) {
                    if(size == 1)
                        writePixel(x+i, y+j, bg);
                    else
                        writeFillRect(x+i*size, y+j*size, size, size, bg);
                }
            }
        }
        if(bg != color) { // If opaque, draw vertical line for last column
            if(size == 1) writeFastVLine(x+5, y, 8, bg);
            else          writeFillRect(x+5*size, y, size, 8*size, bg);
        }
        endWrite();
        
    } else { // Custom font
        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!
        
        c -= (uint8_t)pgm_read_byte(&gfxFont->first);
        GFXglyph *glyph  = &(((GFXglyph *)pgm_read_dword(&gfxFont->glyph))[c]);
        uint8_t  *bitmap = (uint8_t *)pgm_read_dword(&gfxFont->bitmap);
        
        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w  = pgm_read_byte(&glyph->width),
        h  = pgm_read_byte(&glyph->height);
        int8_t   xo = pgm_read_byte(&glyph->xOffset),
        yo = pgm_read_byte(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        int16_t  xo16 = 0, yo16 = 0;
        
        if(size > 1) {
            xo16 = xo;
            yo16 = yo;
        }
        
        // Todo: Add character clipping here
        
        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL infortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.
        
        startWrite();
        for(yy=0; yy<h; yy++) {
            for(xx=0; xx<w; xx++) {
                if(!(bit++ & 7)) {
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if(bits & 0x80) {
                    if(size == 1) {
                        writePixel(x+xo+xx, y+yo+yy, color);
                    } else {
                        writeFillRect(x+(xo16+xx)*size, y+(yo16+yy)*size,
                                      size, size, color);
                    }
                }
                bits <<= 1;
            }
        }
        endWrite();
        
    } // End classic vs custom font
}

void SpriteCanvas::canvasReset()
{
    this->fillScreen(DTRGB(0,0,0));
    this->setCursor(0,this->height());
    textWidth = 0;
}

void SpriteCanvas::copyBuffer(DTRGB* buffer,uint32_t length)
{
    uint32_t copyLength = MIN(length,_width*_height);
    memcpy(_canvasBuffer,buffer,copyLength*sizeof(DTRGB));
}


void SpriteCanvas::drawPixel(int16_t x, int16_t y, const DTRGB& color)
{
    if(_canvasBuffer) {
        if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
        int16_t t;
        switch(rotation) {
            case 1:
                t = x;
                x = WIDTH  - 1 - y;
                y = t;
                break;
            case 2:
                x = WIDTH  - 1 - x;
                y = HEIGHT - 1 - y;
                break;
            case 3:
                t = x;
                x = y;
                y = HEIGHT - 1 - t;
                break;
        }
        _canvasBuffer[x + y * WIDTH] = color;
    }
}

size_t SpriteCanvas::write(uint8_t c)
{
    if(!gfxFont) { // 'Classic' built-in font
        
        if(c == '\n') {                        // Newline?
            cursor_x  = 0;                     // Reset x to zero,
            cursor_y += textSize * 8 + textVSpace;          // advance y one line
        } else if(c != '\r') {                 // Ignore carriage returns
            if(wrap && ((cursor_x + textSize * 6) > _width)) { // Off right?
                cursor_x  = 0;                 // Reset x to zero,
                cursor_y += textSize * 8+ textVSpace;      // advance y one line
            }
            drawChar(cursor_x, cursor_y, c, textColor, textBgColor, textSize);
            cursor_x += textSize * 6 + textHSpace;          // Advance x one char
            if(textWidth < cursor_x) textWidth = cursor_x;
        }
        
    } else { // Custom font
        
        if(c == '\n') {
            cursor_x  = 0;
            cursor_y += (int16_t)textSize *
            (uint8_t)pgm_read_byte(&gfxFont->yAdvance)
            + textVSpace;
        } else if(c != '\r') {
            uint8_t first = pgm_read_byte(&gfxFont->first);
            if((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_dword(
                                                                &gfxFont->glyph))[c - first]);
                uint8_t   w     = pgm_read_byte(&glyph->width),
                h     = pgm_read_byte(&glyph->height);
                if((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                    if(wrap && ((cursor_x + textSize * (xo + w)) > _width)) {
                        cursor_x  = 0;
                        cursor_y += (int16_t)textSize *
                        (uint8_t)pgm_read_byte(&gfxFont->yAdvance)
                        + textVSpace;
                    }
                    drawChar(cursor_x, cursor_y, c, textColor, textBgColor, textSize);
                }
                cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)textSize + textHSpace;
                if(textWidth < cursor_x) textWidth = cursor_x;
            }
        }
        
    }
    return 1;
}


void SpriteCanvas::startWrite(void)
{
    
}

void SpriteCanvas::writePixel(int16_t x, int16_t y,const DTRGB& color)
{
    drawPixel(x, y, color);
}

void SpriteCanvas::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, const DTRGB& color)
{
    fillRect(x,y,w,h,color);
}

void SpriteCanvas::writeFastVLine(int16_t x, int16_t y, int16_t h, const DTRGB& color)
{
    drawFastVLine(x, y, h, color);
}

void SpriteCanvas::writeFastHLine(int16_t x, int16_t y, int16_t w, const DTRGB& color)
{
    drawFastHLine(x, y, w, color);
}

void SpriteCanvas::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const DTRGB& color)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }
    
    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }
    
    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    
    int16_t err = dx / 2;
    int16_t ystep;
    
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    
    for (; x0<=x1; x0++) {
        if (steep) {
            writePixel(y0, x0, color);
        } else {
            writePixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void SpriteCanvas::writeGradientLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,const DTHSV& colorFrom,const DTHSV& colorTo)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }
    
    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }
    
    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    
    int16_t err = dx / 2;
    int16_t ystep;
    
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    
    int16_t maxDiff = x1 - x0;
    for (; x0<=x1; x0++) {
        int16_t nowDiff = x1 - x0;
        float changeRate = nowDiff * 1.0 / maxDiff;
        DTHSV color(colorFrom);
        color.gradientHSV(colorTo, changeRate);
        if (steep) {
            writePixel(y0, x0, color);
        } else {
            writePixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void SpriteCanvas::endWrite(void)
{
    
}

void SpriteCanvas::setRotation(uint8_t r)
{
    rotation = (r & 3);
    switch(rotation) {
        case 0:
        case 2:
            _width  = WIDTH;
            _height = HEIGHT;
            break;
        case 1:
        case 3:
            _width  = HEIGHT;
            _height = WIDTH;
            break;
    }
}

void SpriteCanvas::invertDisplay(bool i)
{
    
}

void SpriteCanvas::drawFastVLine(int16_t x, int16_t y, int16_t h, const DTRGB& color)
{
    startWrite();
    writeLine(x, y, x, y+h-1, color);
    endWrite();
}

void SpriteCanvas::drawFastHLine(int16_t x, int16_t y, int16_t w, const DTRGB& color)
{
    startWrite();
    writeLine(x, y, x+w-1, y, color);
    endWrite();
}

void SpriteCanvas::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, const DTRGB& color)
{
    startWrite();
    for (int16_t i=x; i<x+w; i++) {
        writeFastVLine(i, y, h, color);
    }
    endWrite();
}

void SpriteCanvas::fillScreen(const DTRGB& color)
{
    if(_canvasBuffer) {
        if(color.r == color.g && color.r == color.b) {
            memset(_canvasBuffer, color.r, WIDTH * HEIGHT * 3);
        } else {
            uint32_t i, pixels = WIDTH * HEIGHT;
            for(i=0; i<pixels; i++) _canvasBuffer[i] = color;
        }
    }
}

void SpriteCanvas::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const DTRGB& color)
{
    if(x0 == x1){
        if(y0 > y1) _swap_int16_t(y0, y1);
        drawFastVLine(x0, y0, y1 - y0 + 1, color);
    } else if(y0 == y1){
        if(x0 > x1) _swap_int16_t(x0, x1);
        drawFastHLine(x0, y0, x1 - x0 + 1, color);
    } else {
        startWrite();
        writeLine(x0, y0, x1, y1, color);
        endWrite();
    }
}

void SpriteCanvas::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, const DTRGB& color)
{
    startWrite();
    writeFastHLine(x, y, w, color);
    writeFastHLine(x, y+h-1, w, color);
    writeFastVLine(x, y, h, color);
    writeFastVLine(x+w-1, y, h, color);
    endWrite();
}

void SpriteCanvas::drawCircle(int16_t x0, int16_t y0, int16_t r, const DTRGB& color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    startWrite();
    writePixel(x0  , y0+r, color);
    writePixel(x0  , y0-r, color);
    writePixel(x0+r, y0  , color);
    writePixel(x0-r, y0  , color);
    
    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        writePixel(x0 + x, y0 + y, color);
        writePixel(x0 - x, y0 + y, color);
        writePixel(x0 + x, y0 - y, color);
        writePixel(x0 - x, y0 - y, color);
        writePixel(x0 + y, y0 + x, color);
        writePixel(x0 - y, y0 + x, color);
        writePixel(x0 + y, y0 - x, color);
        writePixel(x0 - y, y0 - x, color);
    }
    endWrite();
}

void SpriteCanvas::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, const DTRGB& color)
{
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;
    
    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            writePixel(x0 + x, y0 + y, color);
            writePixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            writePixel(x0 + x, y0 - y, color);
            writePixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            writePixel(x0 - y, y0 + x, color);
            writePixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            writePixel(x0 - y, y0 - x, color);
            writePixel(x0 - x, y0 - y, color);
        }
    }
}

void SpriteCanvas::fillCircle(int16_t x0, int16_t y0, int16_t r, const DTRGB& color)
{
    startWrite();
    writeFastVLine(x0, y0-r, 2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
    endWrite();
}

void SpriteCanvas::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, const DTRGB& color)
{
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;
    
    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        
        if (cornername & 0x1) {
            writeFastVLine(x0+x, y0-y, 2*y+1+delta, color);
            writeFastVLine(x0+y, y0-x, 2*x+1+delta, color);
        }
        if (cornername & 0x2) {
            writeFastVLine(x0-x, y0-y, 2*y+1+delta, color);
            writeFastVLine(x0-y, y0-x, 2*x+1+delta, color);
        }
    }
}

void SpriteCanvas::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, const DTRGB& color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void SpriteCanvas::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, const DTRGB& color)
{
    int16_t a, b, y, last;
    
    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    
    startWrite();
    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        writeFastHLine(a, y0, b-a+1, color);
        endWrite();
        return;
    }
    
    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa   = 0,
    sb   = 0;
    
    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it
    
    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
         a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
         b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if(a > b) _swap_int16_t(a,b);
        writeFastHLine(a, y, b-a+1, color);
    }
    
    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
         a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
         b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if(a > b) _swap_int16_t(a,b);
        writeFastHLine(a, y, b-a+1, color);
    }
    endWrite();
}

void SpriteCanvas::drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, const DTRGB& color)
{
    startWrite();
    writeFastHLine(x0+radius  , y0    , w-2*radius, color); // Top
    writeFastHLine(x0+radius  , y0+h-1, w-2*radius, color); // Bottom
    writeFastVLine(x0    , y0+radius  , h-2*radius, color); // Left
    writeFastVLine(x0+w-1, y0+radius  , h-2*radius, color); // Right
    // draw four corners
    drawCircleHelper(x0+radius    , y0+radius    , radius, 1, color);
    drawCircleHelper(x0+w-radius-1, y0+radius    , radius, 2, color);
    drawCircleHelper(x0+w-radius-1, y0+h-radius-1, radius, 4, color);
    drawCircleHelper(x0+radius    , y0+h-radius-1, radius, 8, color);
    endWrite();
}

void SpriteCanvas::fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, const DTRGB& color)
{
    startWrite();
    writeFillRect(x0+radius, y0, w-2*radius, h, color);
    
    // draw four corners
    fillCircleHelper(x0+w-radius-1, y0+radius, radius, 1, h-2*radius-1, color);
    fillCircleHelper(x0+radius    , y0+radius, radius, 2, h-2*radius-1, color);
    endWrite();
}

void SpriteCanvas::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, const DTRGB& color)
{
    //    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    //    uint8_t byte = 0;
    //
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++) {
    //            if(i & 7) byte <<= 1;
    //            else      byte   = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
    //            if(byte & 0x80) writePixel(x+i, y, color);
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, const DTRGB& color, const DTRGB& bg)
{
    //    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    //    uint8_t byte = 0;
    //
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            if(i & 7) byte <<= 1;
    //            else      byte   = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
    //            writePixel(x+i, y, (byte & 0x80) ? color : bg);
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, const DTRGB& color)
{
    //    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    //    uint8_t byte = 0;
    //
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            if(i & 7) byte <<= 1;
    //            else      byte   = bitmap[j * byteWidth + i / 8];
    //            if(byte & 0x80) writePixel(x+i, y, color);
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, const DTRGB& color, const DTRGB& bg)
{
    //    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    //    uint8_t byte = 0;
    //
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            if(i & 7) byte <<= 1;
    //            else      byte   = bitmap[j * byteWidth + i / 8];
    //            writePixel(x+i, y, (byte & 0x80) ? color : bg);
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, const DTRGB& color)
{
    //    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    //    uint8_t byte = 0;
    //
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            if(i & 7) byte >>= 1;
    //            else      byte   = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
    //            // Nearly identical to drawBitmap(), only the bit order
    //            // is reversed here (left-to-right = LSB to MSB):
    //            if(byte & 0x01) writePixel(x+i, y, color);
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h)
{
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            writePixel(x+i, y, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h)
{
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            writePixel(x+i, y, bitmap[j * w + i]);
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h)
{
    //    int16_t bw   = (w + 7) / 8; // Bitmask scanline pad = whole byte
    //    uint8_t byte = 0;
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            if(i & 7) byte <<= 1;
    //            else      byte   = pgm_read_byte(&mask[j * bw + i / 8]);
    //            if(byte & 0x80) {
    //                writePixel(x+i, y, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
    //            }
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
    //    int16_t bw   = (w + 7) / 8; // Bitmask scanline pad = whole byte
    //    uint8_t byte = 0;
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            if(i & 7) byte <<= 1;
    //            else      byte   = mask[j * bw + i / 8];
    //            if(byte & 0x80) {
    //                writePixel(x+i, y, bitmap[j * w + i]);
    //            }
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawRGBBitmap(int16_t x, int16_t y, const DTRGB bitmap[], int16_t w, int16_t h)
{
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            writePixel(x+i, y, pgm_read_word(&bitmap[j * w + i]));
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawRGBBitmap(int16_t x, int16_t y, DTRGB *bitmap, int16_t w, int16_t h)
{
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            writePixel(x+i, y, bitmap[j * w + i]);
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawRGBBitmap(int16_t x, int16_t y, const DTRGB bitmap[], const uint8_t mask[], int16_t w, int16_t h)
{
    //    int16_t bw   = (w + 7) / 8; // Bitmask scanline pad = whole byte
    //    uint8_t byte = 0;
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            if(i & 7) byte <<= 1;
    //            else      byte   = pgm_read_byte(&mask[j * bw + i / 8]);
    //            if(byte & 0x80) {
    //                writePixel(x+i, y, pgm_read_word(&bitmap[j * w + i]));
    //            }
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawRGBBitmap(int16_t x, int16_t y, DTRGB *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
    //    int16_t bw   = (w + 7) / 8; // Bitmask scanline pad = whole byte
    //    uint8_t byte = 0;
    //    startWrite();
    //    for(int16_t j=0; j<h; j++, y++) {
    //        for(int16_t i=0; i<w; i++ ) {
    //            if(i & 7) byte <<= 1;
    //            else      byte   = mask[j * bw + i / 8];
    //            if(byte & 0x80) {
    //                writePixel(x+i, y, bitmap[j * w + i]);
    //            }
    //        }
    //    }
    //    endWrite();
}

void SpriteCanvas::drawChar(int16_t x, int16_t y, unsigned char c, const DTRGB& color, const DTRGB& bg, uint8_t size)
{
    if(!gfxFont) { // 'Classic' built-in font
        
        if((x >= _width)            || // Clip right
           (y >= _height)           || // Clip bottom
           ((x + 6 * size - 1) < 0) || // Clip left
           ((y + 8 * size - 1) < 0))   // Clip top
            return;
        
        if( (c >= 176)) c++; // Handle 'classic' charset behavior
        
        startWrite();
        for(int8_t i=0; i<5; i++ ) { // Char bitmap = 5 columns
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            for(int8_t j=0; j<8; j++, line >>= 1) {
                if(line & 1) {
                    if(size == 1)
                        writePixel(x+i, y+j, color);
                    else
                        writeFillRect(x+i*size, y+j*size, size, size, color);
                } else if(bg != color) {
                    if(size == 1)
                        writePixel(x+i, y+j, bg);
                    else
                        writeFillRect(x+i*size, y+j*size, size, size, bg);
                }
            }
        }
        if(bg != color) { // If opaque, draw vertical line for last column
            if(size == 1) writeFastVLine(x+5, y, 8, bg);
            else          writeFillRect(x+5*size, y, size, 8*size, bg);
        }
        endWrite();
        
    } else { // Custom font
        
        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!
        
        c -= (uint8_t)pgm_read_byte(&gfxFont->first);
        GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
        uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
        
        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w  = pgm_read_byte(&glyph->width),
        h  = pgm_read_byte(&glyph->height);
        int8_t   xo = pgm_read_byte(&glyph->xOffset),
        yo = pgm_read_byte(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        int16_t  xo16 = 0, yo16 = 0;
        
        if(size > 1) {
            xo16 = xo;
            yo16 = yo;
        }
        
        // Todo: Add character clipping here
        
        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL infortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.
        
        startWrite();
        for(yy=0; yy<h; yy++) {
            for(xx=0; xx<w; xx++) {
                if(!(bit++ & 7)) {
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if(bits & 0x80) {
                    if(size == 1) {
                        writePixel(x+xo+xx, y+yo+yy, color);
                    } else {
                        writeFillRect(x+(xo16+xx)*size, y+(yo16+yy)*size,
                                      size, size, color);
                    }
                }
                bits <<= 1;
            }
        }
        endWrite();
        
    }
}

void SpriteCanvas::setCursor(int16_t x, int16_t y)
{
    cursor_x = x;
    cursor_y = y;
}

void SpriteCanvas::setTextColor(const DTRGB& c)
{
    textColor = textBgColor = c;
}

void SpriteCanvas::setTextColor(const DTRGB& c, const DTRGB& bg)
{
    textColor   = c;
    textBgColor = bg;
}

void SpriteCanvas::setTextSize(uint8_t s)
{
    textSize = (s > 0) ? s : 1;
}

void SpriteCanvas::setTextWrap(bool w)
{
    wrap = w;
}

void SpriteCanvas::setFont(const GFXfont *f)
{
    if(f) {            // Font struct pointer passed in?
        if(!gfxFont) { // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            cursor_y += 6;
        }
    } else if(gfxFont) { // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        cursor_y -= 6;
    }
    gfxFont = (GFXfont *)f;
}




uint16_t SpriteCanvas::height(void) const
{
    return _height;
}
uint16_t SpriteCanvas::width(void) const
{
    return _width;
}

uint8_t SpriteCanvas::getRotation(void) const
{
    return rotation;
}

int16_t SpriteCanvas::getCursorX(void) const
{
    return cursor_x;
}

int16_t SpriteCanvas::getCursorY(void) const
{
    return cursor_y;
}


NS_DT_END
