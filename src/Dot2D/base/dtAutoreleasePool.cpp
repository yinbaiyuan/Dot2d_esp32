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

#include "dtAutoreleasePool.h"
#include "dtMacros.h"

NS_DT_BEGIN

AutoreleasePool* AutoreleasePool::s_AutoreleasePool = nullptr;

AutoreleasePool* AutoreleasePool::getInstance()
{
    if (s_AutoreleasePool == nullptr)
    {
        s_AutoreleasePool = new AutoreleasePool();
    }
    return s_AutoreleasePool;
}

void AutoreleasePool::destroyInstance()
{
    delete s_AutoreleasePool;
    s_AutoreleasePool = nullptr;
}

void AutoreleasePool::addObject(Ref* object)
{
    _managedObjectArray.push_back(object);
}

void AutoreleasePool::clear()
{
    std::vector<Ref*> releasings;
    releasings.swap(_managedObjectArray);
    for (const auto &obj : releasings)
    {
        obj->release();
    }
}

bool AutoreleasePool::contains(Ref* object) const
{
    for (const auto& obj : _managedObjectArray)
    {
        if (obj == object)
            return true;
    }
    return false;
}

AutoreleasePool::AutoreleasePool()
{
    _managedObjectArray.reserve(50);
}

AutoreleasePool::~AutoreleasePool()
{
    clear();
}

NS_DT_END