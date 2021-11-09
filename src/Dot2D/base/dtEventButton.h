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

#ifndef __DT_EVENT_BUTTON_H__
#define __DT_EVENT_BUTTON_H__

#include "dtEvent.h"

NS_DT_BEGIN

class EventButton : public Event
{
public:

    enum class ButtonEventCode
    {
        BEC_NONE = 0,
        BEC_CLICK = 1,
        BEC_DOUBLE_CLICK,
        BEC_LONG_PRESS_START,
        BEC_LONG_PRESS_STOP,
        BEC_DURING_LONG_PRESS
    };

private:

    int8_t _btnCode;

    ButtonEventCode _btnEventCode;

protected:

    friend class EventListenerButton;
    
public:

    EventButton(int8_t btnCode, ButtonEventCode eventCode = ButtonEventCode::BEC_NONE);

    ~EventButton();

    int8_t btnCode(){return _btnCode;}

    ButtonEventCode eventCode(){return _btnEventCode;}
};



NS_DT_END


#endif //__DT_EVENT_BUTTON_H__