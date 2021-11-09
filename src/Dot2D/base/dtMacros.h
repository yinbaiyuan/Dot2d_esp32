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

#ifndef __DT_MACROS_H__
#define __DT_MACROS_H__

#include <stdint.h>


#ifdef TARGET_OS_MAC
 #import "pgmspace.h"
typedef uint8_t byte;
#include "WString.h"
#endif

#define INLINE inline __attribute__((always_inline))


const int32_t DT_INVALID_INDEX = -1;

/// @name namespace dot2d
/// @{
#ifdef __cplusplus
    #define NS_DT_BEGIN                     namespace dot2d {
    #define NS_DT_END                       }
    #define USING_NS_DT                     using namespace dot2d
    #define NS_DT                           ::dot2d
#else
    #define NS_DT_BEGIN 
    #define NS_DT_END 
    #define USING_NS_DT 
    #define NS_DT
#endif 
//  end of namespace group
/// @}


#ifndef MIN
#define MIN(x,y) (((x) > (y)) ? (y) : (x))
#endif  // MIN

#ifndef MAX
#define MAX(x,y) (((x) < (y)) ? (y) : (x))
#endif  // MAX


#define DT_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define DT_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define DT_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define DT_SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define DT_SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define DT_SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define DT_BREAK_IF(cond)           if(cond) break


#define __DTLOGWITHFUNCTION(s, ...) \
    dot2d::log("%s : %s",__FUNCTION__, dot2d::StringUtils::format(s, ##__VA_ARGS__).c_str())

/// @name dot2d debug
/// @{

//TEMP LOG METHOD

NS_DT_BEGIN
void log(const char * format, ...);
NS_DT_END


#define LOG(format, ...)    dot2d::log(format"\n", ##__VA_ARGS__)
#define LOG_INT(n)          dot2d::log(#n " = %d\n",n)
#define LOG_INT_STR(n,s)    dot2d::log(#n " = %d ----- %s\n",n,s.c_str())
#define LOG_FLOAT(n)        dot2d::log(#n " = %f\n",n)
#define LOG_FLOAT_STR(n,s)  dot2d::log(#n " = %f ----- %s\n",n,s.c_str())
#define LOG_BOOL(n)         dot2d::log(#n " = %s\n",n?"True":"False")
#define LOG_BOOL_STR(n,s)   dot2d::log(#n " = %s ----- %s\n",n?"True":"False",s.c_str())
#define LOG_STR(n)          dot2d::log(#n " = %s\n",n.c_str())
#define LOG_STR_STR(n,s)    dot2d::log(#n " = %s ----- %s\n",n.c_str(),s.c_str())

#if !defined(DOT2D_DEBUG) || DOT2D_DEBUG == 0
#define DTLOG(...)       do {} while (0)
#define DTLOGINFO(...)   do {} while (0)
#define DTLOGERROR(...)  do {} while (0)
#define DTLOGWARN(...)   do {} while (0)

#elif DOT2D_DEBUG == 1
#define DTLOG(format, ...)      dot2d::log(format, ##__VA_ARGS__)
#define DTLOGERROR(format,...)  dot2d::log(format, ##__VA_ARGS__)
#define DTLOGINFO(format,...)   do {} while (0)
#define DTLOGWARN(...) __DTLOGWITHFUNCTION(__VA_ARGS__)

#elif DOT2D_DEBUG > 1
#define DTLOG(format, ...)      dot2d::log(format, ##__VA_ARGS__)
#define DTLOGERROR(format,...)  dot2d::log(format, ##__VA_ARGS__)
#define DTLOGINFO(format,...)   dot2d::log(format, ##__VA_ARGS__)
#define DTLOGWARN(...) __DTLOGWITHFUNCTION(__VA_ARGS__)
#endif // DOT2D_DEBUG

//  end of debug group
/// @}




#ifndef CCASSERT
#if DOT2D_DEBUG > 0
    #define DTASSERT(cond, msg) //TODO
#else
    #define DTASSERT(cond, msg)
#endif
#endif

#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif // FLT_EPSILON

#define DT_REPEAT_FOREVER (UINT_MAX -1)

#define kRepeatForever DT_REPEAT_FOREVER

#define DT_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))




#define STATIC_CREATE(type)         static type* create()                       \
                                    {                                           \
                                        type* ret = new type();                 \
                                        if (ret && ret->init())                 \
                                        {                                       \
                                            ret->autorelease();                 \
                                            return ret;                         \
                                        }                                       \
                                        DT_SAFE_DELETE(ret);                    \
                                        return nullptr;                         \
                                    }





#endif // __DT_MACROS_H__
