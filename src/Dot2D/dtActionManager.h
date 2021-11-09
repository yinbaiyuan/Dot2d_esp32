/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2009      Valentin Milea
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

#ifndef __DT_ACTION_MANAGER_H__
#define __DT_ACTION_MANAGER_H__

#include "dtAction.h"
#include "base/dtVector.h"
#include "base/dtRef.h"

NS_DT_BEGIN

struct _hashElement;

class ActionManager : public Ref
{
public:

    ActionManager();

    ~ActionManager();

    virtual void addAction(Action *action, Node *target, bool paused);

    virtual void removeAllActions();

    virtual void removeAllActionsFromTarget(Node *target);

    virtual void removeAction(Action *action);

    virtual void removeActionByTag(int tag, Node *target);

    virtual void removeAllActionsByTag(int tag, Node *target);

    virtual void removeActionsByFlags(unsigned int flags, Node *target);

    virtual Action* getActionByTag(int tag, const Node *target) const;

    virtual ssize_t getNumberOfRunningActionsInTarget(const Node *target) const;
    
    virtual ssize_t getNumberOfRunningActions() const;

    virtual size_t getNumberOfRunningActionsInTargetByTag(const Node *target, int tag);

    virtual void pauseTarget(Node *target);

    virtual void resumeTarget(Node *target);
    
    virtual Vector<Node*> pauseAllRunningActions();
    
    virtual void resumeTargets(const Vector<Node*>& targetsToResume);
    
    virtual void update(float dt);
    
protected:

    void removeActionAtIndex(int32_t index, struct _hashElement *element);

    void deleteHashElement(struct _hashElement *element);

    void actionAllocWithHashElement(struct _hashElement *element);

protected:

    struct _hashElement    *_targets;

    struct _hashElement    *_currentTarget;

    bool            _currentTargetSalvaged;
};

NS_DT_END

#endif //__DT_ACTION_MANAGER_H__
