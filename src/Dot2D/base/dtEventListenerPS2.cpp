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

#include "dtEventListenerPS2.h"

NS_DT_BEGIN

const std::string EventListenerPS2::LISTENER_ID = "__dt_ps2";

EventListenerPS2::EventListenerPS2()
: onPs2Click(nullptr)
, onPs2Joystick(nullptr)
{

}

EventListenerPS2::~EventListenerPS2()
{

}

bool EventListenerPS2::init()
{
    auto listener = [this](Event* event){
        auto ps2Event = static_cast<EventPS2*>(event);
        switch (ps2Event->eventCode())
        {
        case EventPS2::PS2EventCode::PEC_CLICK:
        {
            if(onPs2Click != nullptr)
            {
                onPs2Click(ps2Event->btnCode(),ps2Event->btnAnalog(),event);
            }
        }
            break;
        case EventPS2::PS2EventCode::PEC_JOYSTICK:
        {
            if(onPs2Joystick!= nullptr)
            {
                onPs2Joystick(ps2Event->btnCode(),ps2Event->btnAnalog(),event);
            }
        }
            break;        
        default:
            break;
        }
    };
    
    if (EventListener::init(Type::PS2, LISTENER_ID, listener))
    {
        return true;
    }
    
    return false;
}

EventListenerPS2* EventListenerPS2::create()
{
    auto ret = new EventListenerPS2();
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

EventListenerPS2* EventListenerPS2::clone()
{
    auto ret = new EventListenerPS2();
    if (ret && ret->init())
    {
        ret->autorelease();
        ret->onPs2Click = onPs2Click;
        ret->onPs2Joystick = onPs2Joystick;
    }
    else
    {
        DT_SAFE_DELETE(ret);
    }
    return ret;
}

bool EventListenerPS2::checkAvailable()
{
    if (onPs2Click == nullptr && 
    onPs2Joystick == nullptr)
    {
        return false;
    }
    return true;
}

NS_DT_END
