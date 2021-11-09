/****************************************************************************
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

#ifndef __DT_EVENT_LISTENER_H__
#define __DT_EVENT_LISTENER_H__

#include <functional>
#include <string>
#include <memory>

#include "dtMacros.h"
#include "dtRef.h"


/**
 * @addtogroup base
 * @{
 */

NS_DT_BEGIN

class Event;
class Node;

class EventListener : public Ref
{
public:

    enum class Type
    {
        BUTTON,
        PS2,
        SENSOR,
        CUSTOM
    };

    typedef std::string ListenerID;

protected:

    std::function<void(Event*)> _onEvent;

    Type _type;

    ListenerID _listenerID; 

    bool _isRegistered;

    int   _fixedPriority;

    Node* _node;

    bool _paused;

    bool _isEnabled;

    friend class EventDispatcher;

protected:

    EventListener();

    bool init(Type t, const ListenerID& listenerID, const std::function<void(Event*)>& callback);

    void setPaused(bool paused) { _paused = paused; }

    bool isPaused() const { return _paused; }

    void setRegistered(bool registered) { _isRegistered = registered; }

    bool isRegistered() const { return _isRegistered; }

    Type getType() const { return _type; }

    const ListenerID& getListenerID() const { return _listenerID; }

    void setFixedPriority(int fixedPriority) { _fixedPriority = fixedPriority; }

    int getFixedPriority() const { return _fixedPriority; }

    void setAssociatedNode(Node* node) { _node = node; }

    Node* getAssociatedNode() const { return _node; }

public:

    virtual ~EventListener();

    virtual bool checkAvailable() = 0;

    virtual EventListener* clone() = 0;

    void setEnabled(bool enabled) { _isEnabled = enabled; }

    bool isEnabled() const { return _isEnabled; }

};


NS_DT_END

// end of base group
/// @}



#endif //__DT_EVENT_LISTENER_H__
