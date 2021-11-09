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

#ifndef __Snake_H__
#define __Snake_H__

#include "Dot2D/dot2d.h"

NS_DT_BEGIN

class Snake : public Scene
{

protected:

    bool init() override;

public:

    STATIC_CREATE(Snake);

};

class SnakeLayer : public Layer
{
protected:
    
    SpriteCanvas * canvas = nullptr;

public:

    enum class Direction
    {
        D_NONE = -1,
        D_UP = 0,
        D_RIGHT = 1,
        D_DOWN = 2,
        D_LEFT = 3
    };

    STATIC_CREATE(SnakeLayer);

    virtual bool initLayer();
    
    void snakeUpdate(float dt);

protected:

    Direction d_controll;

    struct Snake
    {
        Direction dir;
        uint16_t length;
        Vec2 body[256];
    }snake;

    Vec2 food;

    void initSnake();

    bool createFood();
};

NS_DT_END

#endif //__Snake_H__