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

#ifndef __DT_EVENT_LISTENER_PS2_H__
#define __DT_EVENT_LISTENER_PS2_H__

#include "dtEventListener.h"
#include "dtEventPS2.h"

NS_DT_BEGIN

class EventListenerPS2 : public EventListener
{
public:
    
    static const std::string LISTENER_ID;

    std::function<void(uint16_t, uint8_t, Event*)> onPs2Click;

    std::function<void(uint16_t, uint8_t, Event*)> onPs2Joystick;

protected:

    EventListenerPS2();

    ~EventListenerPS2();

    bool init();

public:

    static EventListenerPS2* create();

    /// Overrides
    virtual EventListenerPS2* clone() override;

    virtual bool checkAvailable() override;
    
};




NS_DT_END



#endif //__DT_EVENT_LISTENER_PS2_H__
