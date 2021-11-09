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

#include "dtEventListenerButton.h"


NS_DT_BEGIN

const std::string EventListenerButton::LISTENER_ID = "__dt_button";

EventListenerButton::EventListenerButton()
: onBtnClick(nullptr)
, onBtnDoubleClick(nullptr)
, onBtnLongPressStart(nullptr)
, onBtnLongPressStop(nullptr)
, onBtnDuringLongPress(nullptr)
{

}

EventListenerButton::~EventListenerButton()
{

}

bool EventListenerButton::init()
{
    auto listener = [this](Event* event){
        auto buttonEvent = static_cast<EventButton*>(event);
        switch (buttonEvent->eventCode())
        {
        case EventButton::ButtonEventCode::BEC_CLICK:
        {
            if(onBtnClick != nullptr)
            {
                onBtnClick(buttonEvent->btnCode(),event);
            }
        }
            break;
        case EventButton::ButtonEventCode::BEC_DOUBLE_CLICK:
        {
            if(onBtnDoubleClick!= nullptr)
            {
                onBtnDoubleClick(buttonEvent->btnCode(),event);
            }
        }
            break;
        case EventButton::ButtonEventCode::BEC_LONG_PRESS_START:
        {
            if(onBtnLongPressStart != nullptr)
            {
                onBtnLongPressStart(buttonEvent->btnCode(),event);
            }
        }
            break;
        case EventButton::ButtonEventCode::BEC_LONG_PRESS_STOP:
        {
            if(onBtnLongPressStop != nullptr)
            {
                onBtnLongPressStop(buttonEvent->btnCode(),event);
            }
        }
            break;
        case EventButton::ButtonEventCode::BEC_DURING_LONG_PRESS:
        {
            if(onBtnDuringLongPress != nullptr)
            {
                onBtnDuringLongPress(buttonEvent->btnCode(),event);
            }
        }
            break;
        
        default:
            break;
        }
    };
    
    if (EventListener::init(Type::BUTTON, LISTENER_ID, listener))
    {
        return true;
    }
    
    return false;
}

EventListenerButton* EventListenerButton::create()
{
    auto ret = new EventListenerButton();
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

EventListenerButton* EventListenerButton::clone()
{
    auto ret = new EventListenerButton();
    if (ret && ret->init())
    {
        ret->autorelease();
        ret->onBtnClick = onBtnClick;
        ret->onBtnDoubleClick = onBtnDoubleClick;
        ret->onBtnLongPressStart = onBtnLongPressStart;
        ret->onBtnLongPressStop = onBtnLongPressStop;
        ret->onBtnDuringLongPress = onBtnDuringLongPress;
    }
    else
    {
        DT_SAFE_DELETE(ret);
    }
    return ret;
}

bool EventListenerButton::checkAvailable()
{
    if (onBtnClick == nullptr && 
    onBtnDoubleClick == nullptr && 
    onBtnLongPressStart == nullptr && 
    onBtnLongPressStop == nullptr && 
    onBtnDuringLongPress == nullptr)
    {
        return false;
    }
    return true;
}

NS_DT_END