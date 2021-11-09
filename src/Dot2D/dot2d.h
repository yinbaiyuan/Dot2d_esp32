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

#ifndef __DOT2D_H__
#define __DOT2D_H__

#define DOT2D_VERSION 0x00000100



//base
#include "base/dtMacros.h"
#include "base/dtVector.h"
#include "base/dtTypes.h"
#include "base/dtEventButton.h"
#include "base/dtEventPS2.h"
#include "base/dtEventListenerPS2.h"
#include "base/dtEventListenerButton.h"


//dot2d
#include "dtNode.h"
#include "dtDirector.h"
#include "dtScene.h"
#include "dtLayer.h"
#include "dtSprite.h"
#include "dtActionManager.h"
#include "dtActionInterval.h"
#include "dtActionInstant.h"


//platform
#include "platform/dtFileUtils.h"

NS_DT_BEGIN

const char* dot2dVersion();

NS_DT_END


#endif // __DOT2D_H__
