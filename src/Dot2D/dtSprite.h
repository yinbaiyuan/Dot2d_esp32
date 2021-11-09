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

#ifndef __DT_SPRITE_H__
#define __DT_SPRITE_H__

#include <string>
#include "base/dtMacros.h"
#include "renderer/dtRenderer.h"
#include "dtNode.h"
#include "dtSpriteFrame.h"
#include "dtSpriteCanvas.h"

NS_DT_BEGIN

#define FRAME_DURATION_UNIT_TIME    0.01

class Sprite : public Node
{
protected:

    bool _flippedX;

    bool _flippedY;

protected:

    Sprite();

    virtual ~Sprite();

    virtual bool init() override;

public:

    static Sprite* create();

    bool isFlippedX() const { return _flippedX; }

    void setFlippedX(bool flippedX);

    bool isFlippedY() const { return _flippedY; }

    void setFlippedY(bool flippedY);

    virtual void draw(Renderer *renderer,const Transform& transform) override;

    virtual int8_t nodeClassType() override {return 3;}

};

////////////////////////////////////////////////////////////////////////
//***************************FrameSprite**************************
////////////////////////////////////////////////////////////////////////

class FrameSprite : public Sprite 
{
private:

    SpriteFrame* _spriteFrame = nullptr;

    bool _transparent = false;

    bool _autoSwitch = false;

private:

    void switchUpdate(float dt);

protected:

    virtual bool init(SpriteFrame* spriteFrame);

public:

    FrameSprite();

    ~FrameSprite();

    static FrameSprite* create();

    static FrameSprite* create(const std::string& path);

    static FrameSprite* create(const uint8_t* bitmap,uint32_t size);

    static FrameSprite* create(SpriteFrame* spriteFrame);

    virtual void setSpriteFrame(SpriteFrame* newFrame);

    virtual bool updateFrameData(const uint8_t* bitmap,uint32_t size);

    virtual SpriteFrame* getSpriteFrame(){return _spriteFrame;}

    void setTransparent(bool e){_transparent = e;}

    void setAutoSwitch(bool e);

    virtual void draw(Renderer *renderer,const Transform& transform) override;

};


////////////////////////////////////////////////////////////////////////
//***************************CanvasSprite**************************
////////////////////////////////////////////////////////////////////////

class CanvasSprite : public Sprite 
{
protected:

    SpriteCanvas* _spriteCanvas = nullptr;

    bool _transparent = false;

    virtual bool init(SpriteCanvas* spriteCanvas);
    
    int32_t _paddingTop;
    
    int32_t _paddingRight;
    
    int32_t _paddingBottom;
    
    int32_t _paddingLeft;

public:

    Vec2 contentOffset;
    
    

public:

    CanvasSprite();

    ~CanvasSprite();

    static CanvasSprite* create(SpriteCanvas* spriteCanvas);

    static CanvasSprite* create(uint16_t w,uint16_t h);
    
    void setPaddingTop(int32_t v){_paddingTop = v;}
    
    void setPaddingRight(int32_t v){_paddingRight = v;}
    
    void setPaddingBottom(int32_t v){_paddingBottom = v;}
    
    void setPaddingLeft(int32_t v){_paddingLeft = v;}
    
    Size getRealContentSize() const;

    virtual void setSpriteCanvas(SpriteCanvas* newCanvas);

    virtual SpriteCanvas* getSpriteCanvas(){return _spriteCanvas;}

    virtual void draw(Renderer *renderer,const Transform& transform) override;

    virtual void setTransparent(bool e){_transparent = e;}
    

};

////////////////////////////////////////////////////////////////////////
//***************************TextSprite**************************
////////////////////////////////////////////////////////////////////////



class TextSprite : public CanvasSprite 
{
public:
    
    enum class TextAlign
    {
        TextAlignLeft = 0,
        TextAlignCenter,
        TextAlignRight,
        TextAlignScroll
    };
    
    enum class ScrollType
    {
        None = 0,
        Translate = 1,
        Shake = 2
    };
    
private:

    TextAlign _textAlign = TextAlign::TextAlignLeft;
    
    ScrollType _scrollType = ScrollType::None;
    
    bool _forwardScroll = true;
    
    int32_t _scrollExcessLeft = 0;
    
    int32_t _scrollExcessRight = 0;

protected:

    virtual bool init(uint16_t w, uint16_t h,const std::string& s,TextAlign ta = TextAlign::TextAlignLeft,const GFXfont *f = (const GFXfont *)__null,uint8_t tSize = 1);

    virtual bool init(SpriteCanvas* spriteCanvas) override;
    
public:
    
    TextSprite();

    ~TextSprite();

    static TextSprite* create(const std::string& s,TextAlign ta = TextAlign::TextAlignLeft,const GFXfont *f = (const GFXfont *)__null,uint8_t tSize = 1);

    static TextSprite* create(const Size& spriteSize,const std::string& s,TextAlign ta = TextAlign::TextAlignLeft,const GFXfont *f = (const GFXfont *)__null,uint8_t tSize = 1);

    static TextSprite* create(const Size& spriteSize,const Size& textSize,const std::string& s,TextAlign ta = TextAlign::TextAlignLeft,const GFXfont *f = (const GFXfont *)__null,uint8_t tSize = 1);

    virtual void draw(Renderer *renderer,const Transform& transform) override;
    
    void setTextAlign(TextAlign ta){_textAlign = ta;}
    
    int32_t getAlignOffset(int32_t contentWidth,int32_t textWidth);
    
    void setAutoScroll(ScrollType type,int32_t excess,float interval);
    
    void setAutoScroll(ScrollType type,int32_t excessLeft,int32_t excessRight,float interval);
    
    void scrollUpdate(float dt);

};

NS_DT_END


#endif //__DT_SPRITE_H__
