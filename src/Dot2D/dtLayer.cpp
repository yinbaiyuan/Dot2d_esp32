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

#include "dtLayer.h"
#include "dtDirector.h"

NS_DT_BEGIN

Layer::Layer()
{
    setAnchorPoint(Vec2(0,0));
}

Layer::~Layer()
{

}

bool Layer::init()
{
    Size s = Director::getInstance()->getCanvasSize();
    return init(s.width,s.height);
}

bool Layer::init(uint16_t width,uint16_t height)
{
    setContentSize(Size(width,height));
    return true;
}

Layer *Layer::create()
{
    Layer *ret = new Layer();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        DT_SAFE_DELETE(ret);
        return nullptr;
    }
}

Layer *Layer::create(uint16_t width,uint16_t height)
{
    Layer *ret = new Layer();
    if (ret && ret->init(width,height))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        DT_SAFE_DELETE(ret);
        return nullptr;
    }
}

LayerColor::LayerColor()
{
    
}

LayerColor::~LayerColor()
{
    
}

bool LayerColor::init()
{
    Size s = Director::getInstance()->getCanvasSize();
    return initWithColor(DTRGB(0,0,0),s.width,s.height);
}

bool LayerColor::initWithColor(const DTRGB& color, uint16_t width, uint16_t height)
{
    if(Layer::init())
    {
        _displayedColor = color;
        setContentSize(Size(width,height));
        return true;
    }
    return false;
}

bool LayerColor::initWithColor(const DTRGB& color)
{
    Size s = Director::getInstance()->getCanvasSize();
    return initWithColor(color,s.width,s.height);
}

LayerColor* LayerColor::create()
{
    LayerColor* ret = new LayerColor();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        DT_SAFE_DELETE(ret);
    }
    return ret;
}

LayerColor* LayerColor::create(const DTRGB& color)
{
    LayerColor * layer = new LayerColor();
    if(layer && layer->initWithColor(color))
    {
        layer->autorelease();
        return layer;
    }
    DT_SAFE_DELETE(layer);
    return nullptr;
}

LayerColor* LayerColor::create(const DTRGB& color,uint16_t width,uint16_t height)
{
    LayerColor * layer = new LayerColor();
    if( layer && layer->initWithColor(color,width,height))
    {
        layer->autorelease();
        return layer;
    }
    DT_SAFE_DELETE(layer);
    return nullptr;
}

void LayerColor::draw(Renderer *renderer,const Transform& transform)
{
    for(int32_t x = 0;x<_contentSize.width;x++)
    {
        for(int32_t y = 0;y<_contentSize.height;y++)
        {
            renderer->drawDot(transform,x,y,_displayedColor);
        }
    }
}

NS_DT_END