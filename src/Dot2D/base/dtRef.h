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

#ifndef __DT_REF_H__
#define __DT_REF_H__

#include "dtMacros.h"

/**
 * @addtogroup base
 * @{
 */

NS_DT_BEGIN

class Ref;

class  Clonable
{
public:

    virtual Clonable* clone() const = 0;

    virtual ~Clonable() {};

};

class Ref
{
public:

    void retain();

    void release();

    Ref* autorelease();

    unsigned int getReferenceCount() const;

    virtual int8_t refClassType(){return 0;}

protected:

    Ref();

public:

    virtual ~Ref();

protected:

    unsigned int _referenceCount;

    friend class AutoreleasePool;

};

class Node;

typedef void (Ref::*SEL_CallFunc)();
typedef void (Ref::*SEL_CallFuncN)(Node*);
typedef void (Ref::*SEL_CallFuncND)(Node*, void*);
typedef void (Ref::*SEL_CallFuncO)(Ref*);
typedef void (Ref::*SEL_MenuHandler)(Ref*);
typedef void (Ref::*SEL_SCHEDULE)(float);

#define DT_CALLFUNC_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_CallFunc>(&_SELECTOR)
#define DT_CALLFUNCN_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_CallFuncN>(&_SELECTOR)
#define DT_CALLFUNCND_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_CallFuncND>(&_SELECTOR)
#define DT_CALLFUNCO_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_CallFuncO>(&_SELECTOR)
#define DT_MENU_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_MenuHandler>(&_SELECTOR)
#define DT_SCHEDULE_SELECTOR(_SELECTOR) static_cast<dot2d::SEL_SCHEDULE>(&_SELECTOR)

NS_DT_END

// end of base group
/** @} */

#endif //__DT_REF_H__