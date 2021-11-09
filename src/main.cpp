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

#include <Arduino.h>
#include "Dot2D/dot2d.h"
#include "FastLED.h"
#include "Button.h"

#include "Examples/Matrix/Matrix.h"
#include "Examples/TripleClick/TripleClick.h"
#include "Examples/Snake/Snake.h"

#define BASE_BTN_COUNT        6
//LED矩阵宽
#define MATRIX_WIDTH          32
//LED矩阵高
#define MATRIX_HEIGHT         8
//点阵屏驱动引脚
#define MATRIX_LED_PIN        32
//屏幕最大亮度
#define MATRIX_MAX_BRIGHTNESS 48

uint8_t btnPins[BASE_BTN_COUNT] = {25,26,12,14,27,13};

Button* buttons[BASE_BTN_COUNT];

dot2d::Director* director = nullptr;

void click(void *btn)
{
    Button *button = (Button *)btn;
    dot2d::EventButton event(button->type(),dot2d::EventButton::ButtonEventCode::BEC_CLICK);
    auto dispatcher = dot2d::Director::getInstance()->getEventDispatcher();
    dispatcher->dispatchEvent(&event);
}

//必须要实现的dot2d导演对象代理方法
class MainDelegate : public dot2d::DirectorDelegate
{

    void render()
    {
        FastLED.show();
        // 输出ESP32内存占用情况
        // Serial.printf("-----Free Heap Mem : %d [%.2f%%]-----\n",
        //         ESP.getFreeHeap(),
        //         ESP.getFreeHeap()/(double)ESP.getHeapSize()*100);
        // Serial.printf("-----Free PSRAM Mem: %d [%.2f%%]-----\n",
        //         ESP.getFreePsram(),
        //         ESP.getFreePsram()/(double)ESP.getPsramSize()*100);
    }

    size_t write(uint8_t c)
    {
      return Serial.write(c);
    }

    //返回一个RGB对象的顺序表指针，用于初始化硬件屏幕
    void initMatrix(dot2d::DTRGB *data)
    {
      FastLED.addLeds<WS2812Controller800Khz,MATRIX_LED_PIN, GRB>((CRGB* )data,MATRIX_WIDTH*MATRIX_HEIGHT);
    }

    //用于计算坐标为(x,y)的灯珠在RGB对象顺序表中的具体位置，适配不同排列方式的屏幕，在此处修改
    uint32_t dotOrder(uint16_t x,uint16_t y)
    {
      uint16_t order = y*MATRIX_WIDTH;
      if (y%2==0)
      {
          order+=x;
      }else
      {
          order+=(MATRIX_WIDTH-1-x);
      }
      return order;
    }
};


void setup() {
  //----------------开启串口通信----------------
  Serial.begin(115200);

  //设置WS2812屏幕亮度
  FastLED.setBrightness(MATRIX_MAX_BRIGHTNESS);

  //初始化按键事件
  for (int8_t i = 0; i < BASE_BTN_COUNT; i++)
  {
      buttons[i] = new Button(btnPins[i],true,true);
      buttons[i]->setBtnType((BaseButtonType)i);
      buttons[i]->attachClick(click,buttons[i]);
      buttons[i]->setClickTicks(50);
  }

  //----------------初始化Dot2d引擎及渲染画布----------------
  director = dot2d::Director::getInstance();                      //获取导演对象
  director->setDelegate(new MainDelegate());                      //设置导演代理
  director->setFramesPerSecond(30);                               //设置帧速率
  director->initDotCanvas(MATRIX_WIDTH,MATRIX_HEIGHT);            //初始化导演画布


  //初始化应用场景 不同场景 演示不同的示例

  // director->runWithScene(dot2d::Matrix::create());

  // director->runWithScene(dot2d::TripleClick::create());

  director->runWithScene(dot2d::Snake::create());

  
}

void loop() {
  vTaskDelay(10);
  for (int8_t i = 0; i < BASE_BTN_COUNT; i++)
  {
      buttons[i]->tick();
  }
  director->mainLoop();
}