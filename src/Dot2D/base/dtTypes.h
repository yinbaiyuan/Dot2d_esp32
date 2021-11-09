/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2013 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
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

#ifndef __DT_CCTYPES_H__
#define __DT_CCTYPES_H__

#include "dtMacros.h"

NS_DT_BEGIN

INLINE uint8_t qadd8( uint8_t i, uint8_t j)
{
    uint16_t t = i + j;
    if( t > 255) t = 255;
    return t;
}

INLINE uint8_t qsub8( uint8_t i, uint8_t j)
{
    int16_t t = i - j;
    if( t < 0) t = 0;
    return t;
}

INLINE uint8_t qmul8( uint8_t i, uint8_t j)
{
    int p = ((int)i * (int)(j) );
    if( p > 255) p = 255;
    return p;
}

INLINE void nscale8x3_video( uint8_t& r, uint8_t& g, uint8_t& b, uint8_t scale)
{
    uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
    r = (r == 0) ? 0 : (((int)r * (int)(scale) ) >> 8) + nonzeroscale;
    g = (g == 0) ? 0 : (((int)g * (int)(scale) ) >> 8) + nonzeroscale;
    b = (b == 0) ? 0 : (((int)b * (int)(scale) ) >> 8) + nonzeroscale;
}

INLINE void nscale8x3( uint8_t& r, uint8_t& g, uint8_t& b, uint8_t scale)
{
    uint16_t scale_fixed = scale + 1;
    r = (((uint16_t)r) * scale_fixed) >> 8;
    g = (((uint16_t)g) * scale_fixed) >> 8;
    b = (((uint16_t)b) * scale_fixed) >> 8;
}

INLINE uint8_t scale8( uint8_t i, uint8_t scale)
{
    return (((uint16_t)i) * (1+(uint16_t)(scale))) >> 8;
}

INLINE  uint8_t scale8_video_LEAVING_R1_DIRTY( uint8_t i, uint8_t scale)
{
    uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale)?1:0);
    return j;
}

INLINE uint8_t scale8_LEAVING_R1_DIRTY( uint8_t i, uint8_t scale)
{
    return (((uint16_t)i) * ((uint16_t)(scale)+1)) >> 8;
}

inline uint8_t sqrt16(uint16_t x)
{
    if( x <= 1) {
        return x;
    }

    uint8_t low = 1; // lower bound
    uint8_t hi, mid;

    if( x > 7904) {
        hi = 255;
    } else {
        hi = (x >> 5) + 8; // initial estimate for upper bound
    }

    do {
        mid = (low + hi) >> 1;
        if ((uint16_t)(mid * mid) > x) {
            hi = mid - 1;
        } else {
            if( mid == 255) {
                return 255;
            }
            low = mid + 1;
        }
    } while (hi >= low);

    return low - 1;
}

struct DTRGB;
struct DTHSV;

void hsv2rgb_rainbow( const struct DTHSV& hsv, struct DTRGB& rgb);
void hsv2rgb_rainbow( const struct DTHSV* phsv, struct DTRGB * prgb, int numLeds);


DTHSV rgb2hsv_approximate( const DTRGB& rgb);

DTRGB HeatColor( uint8_t temperature);



struct DTHSV
{
    union
    {
        struct
        {
            union
            {
                uint8_t hue;
                uint8_t h;
            };
            union
            {
                uint8_t saturation;
                uint8_t sat;
                uint8_t s;
            };
            union
            {
                uint8_t value;
                uint8_t val;
                uint8_t v;
            };
        };
        uint8_t raw[3];
    };
    
    INLINE DTHSV()
    :h(0),s(0),v(0)
    {}
    
    INLINE DTHSV( uint8_t ih, uint8_t is, uint8_t iv)
    : h(ih), s(is), v(iv)
    {}
    
    INLINE DTHSV(const DTHSV& rhs)
    :h(rhs.h),s(rhs.s),v(rhs.v)
    {}
    
    INLINE DTHSV& operator= (const DTHSV& rhs)
    {
        h = rhs.h;
        s = rhs.s;
        v = rhs.v;
        return *this;
    }
    
    INLINE DTHSV& setHSV(uint8_t ih, uint8_t is, uint8_t iv)
    {
        h = ih;
        s = is;
        v = iv;
        return *this;
    }
    
    INLINE DTHSV& gradientHSV(const DTHSV& to,float rate)
    {
        h = (to.h - h) * rate + h;
        s = (to.s - s) * rate + s;
        v = (to.v - v) * rate + v;
        return *this;
    }
    
};

typedef enum {
    HUE_RED = 0,
    HUE_ORANGE = 32,
    HUE_YELLOW = 64,
    HUE_GREEN = 96,
    HUE_AQUA = 128,
    HUE_BLUE = 160,
    HUE_PURPLE = 192,
    HUE_PINK = 224
} HSVHue;

struct DTRGB
{
    union
    {
        struct
        {
            union
            {
                uint8_t r;
                uint8_t red;
            };
            union
            {
                uint8_t g;
                uint8_t green;
            };
            union
            {
                uint8_t b;
                uint8_t blue;
            };
        };
        uint8_t raw[3];
    };
    
    INLINE uint8_t& operator[] (uint8_t x)
    {
        return raw[x];
    }
    
    INLINE const uint8_t& operator[] (uint8_t x) const
    {
        return raw[x];
    }
    
    INLINE DTRGB()
    {
    }
    
    INLINE DTRGB( uint8_t ir, uint8_t ig, uint8_t ib)
    : r(ir), g(ig), b(ib)
    {
    }
    
    INLINE DTRGB( uint32_t colorcode)
    : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
    {
    }
    
    INLINE DTRGB(const DTRGB& rhs)
    {
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
    }
    
    INLINE DTRGB(const DTHSV& rhs)
    {
        hsv2rgb_rainbow( rhs, *this);
    }
    
    INLINE DTRGB& operator= (const DTRGB& rhs)
    {
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
        return *this;
    }
    
    INLINE DTRGB& operator= (const uint32_t colorcode)
    {
        r = (colorcode >> 16) & 0xFF;
        g = (colorcode >>  8) & 0xFF;
        b = (colorcode >>  0) & 0xFF;
        return *this;
    }
    
    INLINE DTRGB& setRGB (uint8_t nr, uint8_t ng, uint8_t nb)
    {
        r = nr;
        g = ng;
        b = nb;
        return *this;
    }
    
    INLINE DTRGB& setHSV (uint8_t hue, uint8_t sat, uint8_t val)
    {
        hsv2rgb_rainbow( DTHSV(hue, sat, val), *this);
        return *this;
    }
    
    INLINE DTRGB& setHue (uint8_t hue)
    {
        hsv2rgb_rainbow( DTHSV(hue, 255, 255), *this);
        return *this;
    }
    
    INLINE DTRGB& operator= (const DTHSV& rhs)
    {
        hsv2rgb_rainbow( rhs, *this);
        return *this;
    }
    
    INLINE DTRGB& setColorCode (uint32_t colorcode)
    {
        r = (colorcode >> 16) & 0xFF;
        g = (colorcode >>  8) & 0xFF;
        b = (colorcode >>  0) & 0xFF;
        return *this;
    }
    
    INLINE DTRGB& operator+= (const DTRGB& rhs )
    {
        r = qadd8( r, rhs.r);
        g = qadd8( g, rhs.g);
        b = qadd8( b, rhs.b);
        return *this;
    }
    
    INLINE DTRGB& addToRGB (uint8_t d )
    {
        r = qadd8( r, d);
        g = qadd8( g, d);
        b = qadd8( b, d);
        return *this;
    }
    
    INLINE DTRGB& operator-= (const DTRGB& rhs )
    {
        r = qsub8( r, rhs.r);
        g = qsub8( g, rhs.g);
        b = qsub8( b, rhs.b);
        return *this;
    }
    
    INLINE DTRGB& subtractFromRGB(uint8_t d )
    {
        r = qsub8( r, d);
        g = qsub8( g, d);
        b = qsub8( b, d);
        return *this;
    }
    
    INLINE DTRGB& operator-- ()
    {
        subtractFromRGB(1);
        return *this;
    }
    
    INLINE DTRGB operator-- (int )
    {
        DTRGB retval(*this);
        --(*this);
        return retval;
    }
    
    INLINE DTRGB& operator++ ()
    {
        addToRGB(1);
        return *this;
    }
    
    /// add a constant of '1' from each channel, saturating at 0xFF
    INLINE DTRGB operator++ (int )
    {
        DTRGB retval(*this);
        ++(*this);
        return retval;
    }
    
    INLINE DTRGB& operator/= (uint8_t d )
    {
        r /= d;
        g /= d;
        b /= d;
        return *this;
    }
    
    INLINE DTRGB& operator>>= (uint8_t d)
    {
        r >>= d;
        g >>= d;
        b >>= d;
        return *this;
    }
    
    
    INLINE DTRGB& operator*= (uint8_t d )
    {
        r = qmul8( r, d);
        g = qmul8( g, d);
        b = qmul8( b, d);
        return *this;
    }
    
    INLINE DTRGB& nscale8_video (uint8_t scaledown )
    {
        nscale8x3_video( r, g, b, scaledown);
        return *this;
    }
    
    /// %= is a synonym for nscale8_video.  Think of it is scaling down
    /// by "a percentage"
    INLINE DTRGB& operator%= (uint8_t scaledown )
    {
        nscale8x3_video( r, g, b, scaledown);
        return *this;
    }
    
    /// fadeLightBy is a synonym for nscale8_video( ..., 255-fadefactor)
    INLINE DTRGB& fadeLightBy (uint8_t fadefactor )
    {
        nscale8x3_video( r, g, b, 255 - fadefactor);
        return *this;
    }
    
    /// scale down a RGB to N 256ths of it's current brightness, using
    /// 'plain math' dimming rules, which means that if the low light levels
    /// may dim all the way to 100% black.
    INLINE DTRGB& nscale8 (uint8_t scaledown )
    {
        nscale8x3( r, g, b, scaledown);
        return *this;
    }
    


    
    /// fadeToBlackBy is a synonym for nscale8( ..., 255-fadefactor)
    INLINE DTRGB& fadeToBlackBy (uint8_t fadefactor )
    {
        nscale8x3( r, g, b, 255 - fadefactor);
        return *this;
    }
    
    /// "or" operator brings each channel up to the higher of the two values
    INLINE DTRGB& operator|= (const DTRGB& rhs )
    {
        if( rhs.r > r) r = rhs.r;
        if( rhs.g > g) g = rhs.g;
        if( rhs.b > b) b = rhs.b;
        return *this;
    }
    
    /// "or" operator brings each channel up to the higher of the two values
    INLINE DTRGB& operator|= (uint8_t d )
    {
        if( d > r) r = d;
        if( d > g) g = d;
        if( d > b) b = d;
        return *this;
    }
    
    /// "and" operator brings each channel down to the lower of the two values
    INLINE DTRGB& operator&= (const DTRGB& rhs )
    {
        if( rhs.r < r) r = rhs.r;
        if( rhs.g < g) g = rhs.g;
        if( rhs.b < b) b = rhs.b;
        return *this;
    }
    
    /// "and" operator brings each channel down to the lower of the two values
    INLINE DTRGB& operator&= (uint8_t d )
    {
        if( d < r) r = d;
        if( d < g) g = d;
        if( d < b) b = d;
        return *this;
    }
    
    /// this allows testing a DTRGB for zero-ness
    INLINE operator bool() const
    {
        return r || g || b;
    }
    
    /// invert each channel
    INLINE DTRGB operator- ()
    {
        DTRGB retval;
        retval.r = 255 - r;
        retval.g = 255 - g;
        retval.b = 255 - b;
        return retval;
    }
    

    
    /// getParity returns 0 or 1, depending on the
    /// lowest bit of the sum of the color components.
    INLINE uint8_t getParity()
    {
        uint8_t sum = r + g + b;
        return (sum & 0x01);
    }
    
    INLINE void setParity( uint8_t parity)
    {
        uint8_t curparity = getParity();
        
        if( parity == curparity) return;
        
        if( parity ) {
            // going 'up'
            if( (b > 0) && (b < 255)) {
                if( r == g && g == b) {
                    r++;
                    g++;
                }
                b++;
            } else if( (r > 0) && (r < 255)) {
                r++;
            } else if( (g > 0) && (g < 255)) {
                g++;
            } else {
                if( r == g && g == b) {
                    r ^= 0x01;
                    g ^= 0x01;
                }
                b ^= 0x01;
            }
        } else {
            // going 'down'
            if( b > 1) {
                if( r == g && g == b) {
                    r--;
                    g--;
                }
                b--;
            } else if( g > 1) {
                g--;
            } else if( r > 1) {
                r--;
            } else {
                if( r == g && g == b) {
                    r ^= 0x01;
                    g ^= 0x01;
                }
                b ^= 0x01;
            }
        }
    }
    
    uint16_t u16() const;
    
    DTRGB(uint16_t colorcode);

};



NS_DT_END

#endif //__DT_CCTYPES_H__
