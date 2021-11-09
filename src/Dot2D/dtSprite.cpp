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

#include "dtSprite.h"

NS_DT_BEGIN

Sprite::Sprite()
{
    
}

Sprite::~Sprite()
{
    
}

bool Sprite::init()
{
    return true;
}

Sprite* Sprite::create()
{
    Sprite *sprite = new Sprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    DT_SAFE_DELETE(sprite);
    return nullptr;
}

void Sprite::setFlippedX(bool flippedX)
{
    if (_flippedX != flippedX)
    {
        _flippedX = flippedX;
    }
}

void Sprite::setFlippedY(bool flippedY)
{
    if (_flippedY != flippedY)
    {
        _flippedY = flippedY;
    }
}

void Sprite::draw(Renderer *renderer,const Transform& transform)
{
    //TODO 实现点绘制逻辑
}

////////////////////////////////////////////////////////////////////////
//***************************FrameSprite**************************
////////////////////////////////////////////////////////////////////////

void FrameSprite::switchUpdate(float dt)
{
    if (!_autoSwitch)
    {
        return;
    }
    _spriteFrame->renderNextFrame(true);
    this->scheduleOnce(DT_SCHEDULE_SELECTOR(FrameSprite::switchUpdate),_spriteFrame->getDuration()*FRAME_DURATION_UNIT_TIME);
}

bool FrameSprite::init(SpriteFrame* spriteFrame)
{
    if (spriteFrame == nullptr)
    {
        return false;
    }
    this->setSpriteFrame(spriteFrame);
    return true;
}

FrameSprite::FrameSprite()
{
    
}

FrameSprite::~FrameSprite()
{
    DT_SAFE_RELEASE(_spriteFrame);
}

FrameSprite* FrameSprite::create()
{
    SpriteFrame* frame = SpriteFrame::create();
    if (frame)
    {
        return FrameSprite::create(frame);
    }
    return nullptr;
}

FrameSprite* FrameSprite::create(const std::string& path)
{
    SpriteFrame* frame = SpriteFrame::create(path);
    if (frame)
    {
        return FrameSprite::create(frame);
    }
    return nullptr;
    
}

FrameSprite* FrameSprite::create(const uint8_t* bitmap,uint32_t size)
{
    SpriteFrame* frame = SpriteFrame::create(bitmap,size);
    if (frame)
    {
        return FrameSprite::create(frame);
    }
    return nullptr;
}

FrameSprite* FrameSprite::create(SpriteFrame* spriteFrame)
{
    FrameSprite *sprite = new FrameSprite();
    if (sprite && sprite->init(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    DT_SAFE_DELETE(sprite);
    return nullptr;
}

void FrameSprite::setSpriteFrame(SpriteFrame* newFrame)
{
    if(_spriteFrame == newFrame){ return; }
    DT_SAFE_RELEASE(_spriteFrame);
    _spriteFrame = newFrame;
    _spriteFrame->retain();
    _contentSize = Size(_spriteFrame->width(),_spriteFrame->height());
}

bool FrameSprite::updateFrameData(const uint8_t* bitmap,uint32_t size)
{
    if(!_spriteFrame){ return false; }
    bool res = _spriteFrame->decode(bitmap,size);
    _contentSize = Size(_spriteFrame->width(),_spriteFrame->height());
    return res;
}

void FrameSprite::setAutoSwitch(bool e)
{
    _autoSwitch = e;
    if (_autoSwitch)
    {
        this->scheduleOnce(DT_SCHEDULE_SELECTOR(FrameSprite::switchUpdate),_spriteFrame->getDuration()*FRAME_DURATION_UNIT_TIME);
    }
}

void FrameSprite::draw(Renderer *renderer,const Transform& transform)
{
    if(!_spriteFrame){ return; }
    for(int32_t x = 0;x<_contentSize.width;x++)
    {
        for(int32_t y = 0;y<_contentSize.height;y++)
        {
            if(_transparent)
            {
                DTRGB color = _spriteFrame->colorWithXY(x,y);
                if(color.r + color.g + color.b > 0)
                {
                    renderer->drawDot(transform,x,y,color);
                }
            }else
            {
                renderer->drawDot(transform,x,y,_spriteFrame->colorWithXY(x,y));
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
//***************************SpriteCanvas**************************
////////////////////////////////////////////////////////////////////////

bool CanvasSprite::init(SpriteCanvas* spriteCanvas)
{
    if (spriteCanvas == nullptr)
    {
        return false;
    }
    this->setSpriteCanvas(spriteCanvas);
    return true;
}

CanvasSprite::CanvasSprite()
{
    contentOffset.x = 0;
    contentOffset.y = 0;
    _paddingTop = 0;
    _paddingRight = 0;
    _paddingBottom = 0;
    _paddingLeft = 0;
}

CanvasSprite::~CanvasSprite()
{
    DT_SAFE_RELEASE(_spriteCanvas);
}

CanvasSprite* CanvasSprite::create(SpriteCanvas* spriteCanvas)
{
    CanvasSprite *sprite = new CanvasSprite();
    if (sprite && sprite->init(spriteCanvas))
    {
        sprite->autorelease();
        return sprite;
    }
    DT_SAFE_DELETE(sprite);
    return nullptr;
}

CanvasSprite* CanvasSprite::create(uint16_t w,uint16_t h)
{
    SpriteCanvas *canvas = SpriteCanvas::create(w,h);
    return CanvasSprite::create(canvas);
}

Size CanvasSprite::getRealContentSize() const
{
    Size size = Sprite::getContentSize();
    size.width -= (_paddingLeft + _paddingRight);
    size.height -= (_paddingTop + _paddingBottom);
    return size;
}

void CanvasSprite::setSpriteCanvas(SpriteCanvas* newCanvas)
{
    if(_spriteCanvas == newCanvas){ return; }
    DT_SAFE_RELEASE(_spriteCanvas);
    _spriteCanvas = newCanvas;
    _spriteCanvas->retain();
    _contentSize = Size(_spriteCanvas->width(),_spriteCanvas->height());
}

void CanvasSprite::draw(Renderer *renderer,const Transform& transform)
{
    if(!_spriteCanvas){ return; }
    for(int32_t x = 0;x<_contentSize.width;x++)
    {
        for(int32_t y = 0;y<_contentSize.height;y++)
        {
            if(_transparent)
            {
                DTRGB color = _spriteCanvas->colorWithXY(x,y);
                if(color.r + color.g + color.b > 0)
                {
                    renderer->drawDot(transform,x,y,color);
                }
            }else
            {
                renderer->drawDot(transform,x,y,_spriteCanvas->colorWithXY(x,y));
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////
//***************************TextSprite**************************
////////////////////////////////////////////////////////////////////////

bool TextSprite::init(uint16_t w, uint16_t h,const std::string& s,TextAlign ta,const GFXfont *f,uint8_t tSize)
{
    return true;
}

bool TextSprite::init(SpriteCanvas* spriteCanvas)
{
    if (spriteCanvas == nullptr)
    {
        return false;
    }
    this->setSpriteCanvas(spriteCanvas);
    return true;
}

TextSprite::TextSprite()
{
    
}

TextSprite::~TextSprite()
{
    
}

TextSprite* TextSprite::create(const std::string& s,TextAlign ta,const GFXfont *f,uint8_t tSize)
{
    SpriteCanvas *canvas = SpriteCanvas::create(s,f,tSize);
    if (canvas)
    {
        TextSprite *sprite = new TextSprite();
        if(sprite && sprite->init(canvas))
        {
            sprite->setContentSize(Size(canvas->width(),canvas->height()));
            sprite->setTextAlign(ta);
            sprite->autorelease();
            return sprite;
        }
        DT_SAFE_DELETE(sprite);
    }
    return nullptr;
}

TextSprite* TextSprite::create(const Size& spriteSize,const std::string& s,TextAlign ta,const GFXfont *f,uint8_t tSize)
{
    SpriteCanvas *canvas = SpriteCanvas::create(s,f,tSize);
    if (canvas)
    {
        TextSprite *sprite = new TextSprite();
        if(sprite && sprite->init(canvas))
        {
            sprite->setContentSize(Size(spriteSize.width,spriteSize.height));
            sprite->setTextAlign(ta);
            sprite->autorelease();
            return sprite;
        }
        DT_SAFE_DELETE(sprite);
    }
    return nullptr;
}

TextSprite* TextSprite::create(const Size& spriteSize,const Size& textSize,const std::string& s,TextAlign ta,const GFXfont *f,uint8_t tSize)
{
    SpriteCanvas *canvas = SpriteCanvas::create(textSize.width,textSize.height,s,f,tSize);
    if (canvas)
    {
        TextSprite *sprite = new TextSprite();
        if(sprite && sprite->init(canvas))
        {
            sprite->setContentSize(Size(spriteSize.width,spriteSize.height));
            sprite->setTextAlign(ta);
            sprite->autorelease();
            return sprite;
        }
        DT_SAFE_DELETE(sprite);
    }
    return nullptr;
}

void TextSprite::draw(Renderer *renderer,const Transform& transform)
{
    if(!_spriteCanvas){ return; }
    Size contentSize = getContentSize();
    Size realSize = getRealContentSize();
    
    int16_t alignOffset = getAlignOffset(realSize.width,_spriteCanvas->getTextWidth());
    for(int32_t x = _paddingLeft;x<contentSize.width-_paddingRight;x++)
    {
        for(int32_t y = _paddingTop;y<contentSize.height-_paddingBottom;y++)
        {
            Vec2 contentPos(x,y);
            contentPos.x += (-_paddingLeft - contentOffset.x - alignOffset);
            contentPos.y += (-_paddingTop - contentOffset.y);
            if (_spriteCanvas->posInRect(contentPos.x,contentPos.y))
            {
                if(_transparent)
                {
                    DTRGB color = _spriteCanvas->colorWithXY(contentPos.x,contentPos.y);
                    if(color.r + color.g + color.b > 0)
                    {
                        renderer->drawDot(transform,x,y,color);
                    }
                }else
                {
                    DTRGB color = _spriteCanvas->colorWithXY(contentPos.x,contentPos.y);
                    renderer->drawDot(transform,x,y,color);
                }
            }
        }
    }
}

int32_t TextSprite::getAlignOffset(int32_t contentWidth,int32_t textWidth)
{
    int16_t res = 0;
    switch (_textAlign)
    {
        case TextAlign::TextAlignLeft:
            res = 0;
            break;
        case TextAlign::TextAlignCenter:
            res = (contentWidth  - textWidth) / 2;
            break;
        case TextAlign::TextAlignRight:
            res = contentWidth  - textWidth;
            break;
        default:
            break;
    }
    return res;
}

void TextSprite::setAutoScroll(ScrollType type,int32_t excess,float interval)
{
    this->setAutoScroll(type, excess, excess, interval);
}

void TextSprite::setAutoScroll(ScrollType type,int32_t excessLeft,int32_t excessRight,float interval)
{
    if (_scrollType == type) {
        return;
    }
    _scrollType = type;
    _scrollExcessLeft = excessLeft;
    _scrollExcessRight = excessRight;
    if (_scrollType == ScrollType::None) {
        this->unschedule(DT_SCHEDULE_SELECTOR(TextSprite::scrollUpdate));
    }else
    {
        this->schedule(DT_SCHEDULE_SELECTOR(TextSprite::scrollUpdate), interval);
    }
}

void TextSprite::scrollUpdate(float dt)
{
    Size realSize = getRealContentSize();
    if (realSize.width >= _spriteCanvas->getTextWidth()) {
        return;
    }
    int16_t alignOffset = getAlignOffset(realSize.width,_spriteCanvas->getTextWidth());
    switch (_scrollType) {
        case ScrollType::Translate:
            contentOffset.x--;
            if (_spriteCanvas->getTextWidth() + contentOffset.x + alignOffset + _scrollExcessRight< realSize.width) {
                contentOffset.x = -alignOffset + _scrollExcessLeft;
            }
            break;
        case ScrollType::Shake:
            if (_forwardScroll) {
                contentOffset.x--;
                if(_spriteCanvas->getTextWidth() + contentOffset.x + alignOffset + _scrollExcessRight <= realSize.width) {
                    _forwardScroll = false;
                }
            }else
            {
                contentOffset.x++;
                if (contentOffset.x >= -alignOffset + _scrollExcessLeft) {
                    _forwardScroll = true;
                }
            }
            break;
        default:
            break;
    }
}

NS_DT_END
