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

#include "TripleClick.h"

NS_DT_BEGIN

INLINE uint16_t GetUint16(const uint8_t* buffer)
{
    return (uint16_t(buffer[0]) << 8) 
    + uint16_t(buffer[1]);
}

const uint8_t icon_thumb[] PROGMEM =
{
    0x00,0x36,
    0x44,0x47,0x46,0x11,0x08,0x08,0x01,0x00,
    0x00,0x00,0xe8,0x65,0x8a,0x2c,0x00,0x0a,
    0x00,0x04,0x08,0x04,0x40,0x00,0xdf,0xdf,
    0xdf,0xde,0x2c,0x00,0x0a,0x03,0x03,0x03,
    0x01,0x40,0x20,0x00,0x00,0xe0,0x2c,0x00,
    0xc8,0x00,0x02,0x08,0x06,0x40,0x00,0x0c,
    0x1c,0xdf,0xdf,0xdf,0xde,0x96
};

const uint8_t icon_coin[] PROGMEM =
{
    0x00,0x2f,
    0x44,0x47,0x46,0x11,0x08,0x08,0x01,0x00,
    0x00,0x00,0xe8,0x65,0x8a,0x2c,0x00,0x64,
    0x00,0x00,0x08,0x08,0x40,0x00,0x7e,0xfb,
    0x83,0xef,0x83,0xab,0xef,0x7e,0x2c,0x00,
    0x64,0x01,0x01,0x06,0x06,0x40,0x20,0x00,
    0x00,0xfa,0x0e,0xe0,0xab,0xb0,0x96
};

const uint8_t icon_collect[] PROGMEM =
{
    0x00,0x43,
    0x44,0x47,0x46,0x11,0x08,0x08,0x01,0x00,
    0x00,0x00,0xe8,0x65,0x8a,0x2c,0x00,0x0a,
    0x00,0x00,0x08,0x08,0x40,0x00,0x18,0x18,
    0x3c,0xff,0x7e,0x3c,0x7e,0xc3,0x2c,0x00,
    0x0a,0x00,0x07,0x03,0x01,0x40,0x20,0x00,
    0x00,0x60,0x2c,0x00,0x0a,0x05,0x07,0x03,
    0x01,0x40,0x20,0x00,0x00,0xc0,0x2c,0x00,
    0x0a,0x00,0x07,0x03,0x01,0x40,0x20,0x00,
    0x00,0xc0,0x96
};



bool TripleClick::init()
{
    TripleClickLayer *rootLayer = TripleClickLayer::create();
    rootLayer->setContentSize(Size(32,8));
    this->addChild(rootLayer);
    rootLayer->initLayer();
    return true;
}

bool TripleClickLayer::initLayer()
{
    FrameSprite* thumb = FrameSprite::create(icon_thumb+2,GetUint16(icon_thumb));
    this->addChild(thumb);
    thumb->setPosition(0,6);
    thumb->setAutoSwitch(true);
    MoveBy *moveBy1 = MoveBy::create(0.5,Vec2(0,-7));
    thumb->runAction(moveBy1);

    FrameSprite* coin = FrameSprite::create(icon_coin+2,GetUint16(icon_coin));
    this->addChild(coin);
    coin->setPosition(12,-6);
    coin->setAutoSwitch(true);
    MoveBy *moveBy2 = MoveBy::create(0.8,Vec2(0,6));
    coin->runAction(moveBy2);

    FrameSprite* collect = FrameSprite::create(icon_collect+2,GetUint16(icon_collect));
    this->addChild(collect);
    collect->setPosition(32,0);
    collect->setAutoSwitch(true);
    MoveBy *moveBy3 = MoveBy::create(1.2,Vec2(-8,0));
    collect->runAction(moveBy3);

    return true;
}


NS_DT_END