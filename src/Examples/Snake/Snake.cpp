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

#include "Snake.h"

NS_DT_BEGIN

bool Snake::init()
{
    SnakeLayer *rootLayer = SnakeLayer::create();
    rootLayer->setContentSize(Size(32,8));
    this->addChild(rootLayer);
    rootLayer->initLayer();
    return true;
}

bool SnakeLayer::initLayer()
{
    Size size = this->getContentSize();
    CanvasSprite *canvasSprite = CanvasSprite::create(size.width,size.height);
    this->addChild(canvasSprite);
    this->scheduleUpdate();
    canvas = canvasSprite->getSpriteCanvas();

    auto listener = EventListenerButton::create();
    listener ->onBtnClick = [&](int8_t keyCode , Event * event ){
        if (keyCode >= 0 && keyCode <= 3)
        {
            d_controll = (Direction)keyCode;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener, this );
    this->initSnake();
    this->createFood();
    this->schedule(DT_SCHEDULE_SELECTOR(SnakeLayer::snakeUpdate),0.2);
    return true;
}

void SnakeLayer::snakeUpdate(float dt)
{
    Size size = this->getContentSize();
    Direction nextDir = snake.dir;
    int8_t dirDiff = abs((int8_t)nextDir - (int8_t)d_controll);
    if (dirDiff != 0 && dirDiff != 2)
    {
        nextDir = d_controll;
        snake.dir = nextDir;
    }
    Vec2 nextPos = snake.body[0];
    switch (nextDir)
    {
    case Direction::D_UP:{nextPos.y--;if(nextPos.y<0)nextPos.y=size.height-1;}break;
    case Direction::D_RIGHT:{nextPos.x++;if(nextPos.x>=size.width)nextPos.x=0;}break;
    case Direction::D_DOWN:{nextPos.y++;if(nextPos.y>=size.height)nextPos.y=0;}break;
    case Direction::D_LEFT:{nextPos.x--;if(nextPos.x<0)nextPos.x=size.width-1;}break;
    default:
        break;
    }
    for (uint16_t i = 0; i < snake.length; i++)
    {
        Vec2 tempPos = snake.body[i];
        snake.body[i] = nextPos;
        nextPos = tempPos;
    }
    if (snake.body[0] == food)
    {
        snake.body[snake.length] = nextPos;
        snake.length++;
        this->createFood();
    }
    for (uint16_t i = 4; i < snake.length; i++)
    {
        if (snake.body[0]==snake.body[i])
        {
            this->initSnake();
            this->createFood();
            break;
        }
    }
    //清楚屏幕内容
    canvas->canvasReset();
    //绘制蛇身
    for (uint16_t i = 0; i < snake.length; i++)
    {
        canvas->drawPixel(snake.body[i].x,snake.body[i].y,DTRGB(200,200,200));
    }
    //绘制食物
    canvas->drawPixel(food.x,food.y,DTRGB(200,0,0));
}

void SnakeLayer::initSnake()
{
    snake.length = 3;
    snake.dir = Direction::D_RIGHT;
    for (uint16_t i = 0; i < snake.length; i++)
    {
        snake.body[i] = Vec2(10-i,3);
    }
    d_controll = Direction::D_RIGHT;
}

bool SnakeLayer::createFood()
{
    //TODO 当蛇身体过于长时，完全随机生成食物位置，可能导致死循环，可以通过维护一个无蛇身位置表来优化
    Size size = this->getContentSize();
    bool pass = true;
    do
    {
        food.x = random(0,size.width-1);
        food.y = random(0,size.height-1);
        pass = true;
        for (uint16_t i = 0; i < snake.length; i++)
        {
            if (food == snake.body[i])
            {
                pass = false;
            }
        }
    } while (!pass);
    return true;
}

NS_DT_END