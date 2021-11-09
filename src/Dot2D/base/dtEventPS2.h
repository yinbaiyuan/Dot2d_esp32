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

#ifndef __DT_EVENT_PS2_H__
#define __DT_EVENT_PS2_H__

#include "dtEvent.h"

NS_DT_BEGIN

class EventPS2 : public Event
{
public:

    enum class PS2EventCode
    {
        PEC_NONE = 0,
        PEC_CLICK = 1,
        PEC_JOYSTICK,
    };

private:

    uint16_t _btnCode;

    uint8_t _btnAnalog;

    PS2EventCode _ps2EventCode;

protected:

    friend class EventListenerButton;
    
public:

    EventPS2(uint16_t btnCode, uint8_t btnAnalog, PS2EventCode eventCode = PS2EventCode::PEC_NONE);

    ~EventPS2();

    uint16_t btnCode(){return _btnCode;}

    uint8_t btnAnalog(){return _btnAnalog;}

    PS2EventCode eventCode(){return _ps2EventCode;}
};



NS_DT_END


#endif //__DT_EVENT_PS2_H__
