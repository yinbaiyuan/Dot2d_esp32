/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
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

#ifndef __DT_ACTION_H__
#define __DT_ACTION_H__

#include "base/dtRef.h"
#include "math/Vec2.h"
#include "math/dtGeometry.h"
#include <string>

NS_DT_BEGIN

class Node;

enum {
    kActionUpdate
};

/**
 * @addtogroup actions
 * @{
 */

/** 
 * @brief Base class for Action objects.
 */

class Action : public Ref, public Clonable
{
public:
    /** Default tag used for all the actions. */
    static const int INVALID_TAG = -1;

    virtual Action* clone() const { return nullptr; }

    virtual Action* reverse() const { return nullptr; }

    virtual bool isDone() const;

    virtual void startWithTarget(Node *target);

    virtual void stop();

    virtual void step(float dt);

    virtual void update(float time);

    Node* getTarget() const { return _target; }

    void setTarget(Node *target) { _target = target; }

    Node* getOriginalTarget() const { return _originalTarget; }

    void setOriginalTarget(Node *originalTarget) { _originalTarget = originalTarget; }

    int getTag() const { return _tag; }

    void setTag(int tag) { _tag = tag; }

    unsigned int getFlags() const { return _flags; }

    void setFlags(unsigned int flags) { _flags = flags; }

    virtual int8_t actionClassType(){return 0;}

protected:

    Action();

    virtual ~Action();

protected:

    Node    *_originalTarget;

    Node    *_target;

    int     _tag;

    unsigned int _flags;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(Action);
};

class FiniteTimeAction : public Action
{
public:

    float getDuration() const { return _duration; }

    void setDuration(float duration) { _duration = duration; }

    virtual FiniteTimeAction* reverse() const override { return nullptr; }
    virtual FiniteTimeAction* clone() const override { return nullptr; }


protected:
    FiniteTimeAction()
    : _duration(0)
    {}
    virtual ~FiniteTimeAction(){}

protected:

    float _duration;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(FiniteTimeAction);
};

class ActionInterval;
class RepeatForever;

class Speed : public Action
{
public:

    static Speed* create(ActionInterval* action, float speed);

    float getSpeed() const { return _speed; }

    void setSpeed(float speed) { _speed = speed; }

    void setInnerAction(ActionInterval *action);

    ActionInterval* getInnerAction() const { return _innerAction; }

    virtual Speed* clone() const override;

    virtual Speed* reverse() const override;

    virtual void startWithTarget(Node* target) override;

    virtual void stop() override;

    virtual void step(float dt) override;

    virtual bool isDone() const  override;
    
protected:

    Speed();

    virtual ~Speed(void);

    bool initWithAction(ActionInterval *action, float speed);

protected:

    float _speed;

    ActionInterval *_innerAction;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(Speed);
};

class Follow : public Action
{
public:
    
    static Follow* create(Node *followedNode, const Rect& rect = Rect::ZERO);
    
    static Follow* createWithOffset(Node* followedNode,float xOffset,float yOffset,const Rect& rect = Rect::ZERO);
    
    bool isBoundarySet() const { return _boundarySet; }

    void setBoundarySet(bool value) { _boundarySet = value; }

    virtual Follow* clone() const override;

    virtual Follow* reverse() const override;

    virtual void step(float dt) override;

    virtual bool isDone() const override;

    virtual void stop() override;

protected:

    Follow()
    : _followedNode(nullptr)
    , _boundarySet(false)
    , _boundaryFullyCovered(false)
    , _leftBoundary(0.0)
    , _rightBoundary(0.0)
    , _topBoundary(0.0)
    , _bottomBoundary(0.0)
    , _offsetX(0.0)
    , _offsetY(0.0)
    , _worldRect(Rect::ZERO)
    {}

    virtual ~Follow();
    
    bool initWithTarget(Node *followedNode, const Rect& rect = Rect::ZERO);
    
    bool initWithTargetAndOffset(Node *followedNode,float xOffset,float yOffset,const Rect& rect = Rect::ZERO);

protected:

    Node *_followedNode;

    bool _boundarySet;

    bool _boundaryFullyCovered;

    Vec2 _halfScreenSize;

    Vec2 _fullScreenSize;

    float _leftBoundary;

    float _rightBoundary;

    float _topBoundary;

    float _bottomBoundary;
    
    float _offsetX;
    
    float _offsetY;
    
    Rect _worldRect;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(Follow);
};

// end of actions group
/// @}

NS_DT_END


#endif //__DT_ACTION_H__