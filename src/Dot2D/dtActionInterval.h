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

#ifndef __DT_ACTION_INTERVAL_H__
#define __DT_ACTION_INTERVAL_H__

#include <vector>
#include "base/dtConfig.h"
#include "base/dtVector.h"
#include "base/dtTypes.h"
#include "dtAction.h"
#include <stdarg.h>

NS_DT_BEGIN

class ActionInterval : public FiniteTimeAction
{
protected:
    
    float _elapsed;
    
    bool _firstTick;
    
    bool _done;

protected:

    bool initWithDuration(float d);

public:

    float getElapsed() { return _elapsed; }

    void setAmplitudeRate(float amp);

    float getAmplitudeRate();

    virtual bool isDone() const override;

    virtual void step(float dt) override;

    virtual void startWithTarget(Node *target) override;

    virtual ActionInterval* reverse() const override { return nullptr; }

    virtual ActionInterval *clone() const override { return nullptr; }

    virtual int8_t actionClassType() override {return 2;}

};

class Sequence : public ActionInterval
{
public:

    static Sequence* create(FiniteTimeAction *action1, ...);

    static Sequence* create(const Vector<FiniteTimeAction*>& arrayOfActions);

    static Sequence* createWithVariableList(FiniteTimeAction *action1, va_list args);
    
    static Sequence* createWithTwoActions(FiniteTimeAction *actionOne, FiniteTimeAction *actionTwo);

    virtual Sequence* clone() const override;

    virtual Sequence* reverse() const override;

    virtual void startWithTarget(Node *target) override;

    virtual void stop() override;

    virtual bool isDone() const override;

    virtual void update(float t) override;
    
protected:

    Sequence();

    virtual ~Sequence();

    bool initWithTwoActions(FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo);

    bool init(const Vector<FiniteTimeAction*>& arrayOfActions);

protected:

    FiniteTimeAction *_actions[2];

    float _split;

    int _last;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(Sequence);

};

class Repeat : public ActionInterval
{
public:
    static Repeat* create(FiniteTimeAction *action, unsigned int times);

    void setInnerAction(FiniteTimeAction *action)
    {
        if (_innerAction != action)
        {
            DT_SAFE_RETAIN(action);
            DT_SAFE_RELEASE(_innerAction);
            _innerAction = action;
        }
    }

    FiniteTimeAction* getInnerAction()
    {
        return _innerAction;
    }

    virtual Repeat* clone() const override;

    virtual Repeat* reverse() const override;

    virtual void startWithTarget(Node *target) override;

    virtual void stop(void) override;

    virtual void update(float dt) override;

    virtual bool isDone(void) const override;
    
protected:

    Repeat() {}

    virtual ~Repeat();

    bool initWithAction(FiniteTimeAction *pAction, unsigned int times);

protected:

    unsigned int _times;

    unsigned int _total;

    float _nextDt;

    bool _actionInstant;

    FiniteTimeAction *_innerAction;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(Repeat);
};

class RepeatForever : public ActionInterval
{
public:

    static RepeatForever* create(ActionInterval *action);

    void setInnerAction(ActionInterval *action)
    {
        if (_innerAction != action)
        {
            DT_SAFE_RELEASE(_innerAction);
            _innerAction = action;
            DT_SAFE_RETAIN(_innerAction);
        }
    }

    ActionInterval* getInnerAction()
    {
        return _innerAction;
    }

    virtual RepeatForever* clone() const override;

    virtual RepeatForever* reverse(void) const override;

    virtual void startWithTarget(Node* target) override;

    virtual void step(float dt) override;

    virtual bool isDone(void) const override;
    
protected:

    RepeatForever()
    : _innerAction(nullptr)
    {}

    virtual ~RepeatForever();

    bool initWithAction(ActionInterval *action);

protected:

    ActionInterval *_innerAction;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(RepeatForever);
};

class Spawn : public ActionInterval
{
public:

    static Spawn* create(FiniteTimeAction *action1, ...);

    static Spawn* createWithVariableList(FiniteTimeAction *action1, va_list args);

    static Spawn* create(const Vector<FiniteTimeAction*>& arrayOfActions);

    static Spawn* createWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2);

    virtual Spawn* clone() const override;

    virtual Spawn* reverse() const override;

    virtual void startWithTarget(Node *target) override;

    virtual void stop() override;

    virtual void update(float time) override;
    
protected:

    Spawn();

    virtual ~Spawn();

    bool initWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2);

    bool init(const Vector<FiniteTimeAction*>& arrayOfActions);

protected:

    FiniteTimeAction *_one;

    FiniteTimeAction *_two;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(Spawn);
};

class MoveBy : public ActionInterval
{
public:

    static MoveBy* create(float duration, const Vec2& deltaPosition);

    virtual MoveBy* clone() const override;

    virtual MoveBy* reverse() const  override;

    virtual void startWithTarget(Node *target) override;

    virtual void update(float time) override;
    
protected:

    MoveBy(){}

    virtual ~MoveBy() {}

    bool initWithDuration(float duration, const Vec2& deltaPosition);

protected:

    Vec2 _positionDelta;

    Vec2 _startPosition;

    Vec2 _previousPosition;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(MoveBy);
};

class MoveTo : public MoveBy
{
public:

    static MoveTo* create(float duration, const Vec2& position);

    virtual MoveTo* clone() const override;

    virtual MoveTo* reverse() const  override;

    virtual void startWithTarget(Node *target) override;
    
protected:

    MoveTo() {}

    virtual ~MoveTo() {}

    bool initWithDuration(float duration, const Vec2& position);

protected:
    Vec2 _endPosition;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(MoveTo);
};

class Blink : public ActionInterval
{
public:

    static Blink* create(float duration, int blinks);

    virtual Blink* clone() const override;

    virtual Blink* reverse() const override;

    virtual void update(float time) override;

    virtual void startWithTarget(Node *target) override;

    virtual void stop() override;
    
protected:

    Blink() {}

    virtual ~Blink() {}

    bool initWithDuration(float duration, int blinks);
    
protected:

    int _times;

    bool _originalState;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(Blink);
};

class FadeTo : public ActionInterval
{
public:

    static FadeTo* create(float duration, uint8_t opacity);

    virtual FadeTo* clone() const override;

    virtual FadeTo* reverse() const override;

    virtual void startWithTarget(Node *target) override;

    virtual void update(float time) override;
    
protected:

    FadeTo() {}

    virtual ~FadeTo() {}

    bool initWithDuration(float duration, uint8_t opacity);

protected:

    uint8_t _toOpacity;

    uint8_t _fromOpacity;

    friend class FadeOut;

    friend class FadeIn;

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(FadeTo);
};

class FadeIn : public FadeTo
{
public:

    static FadeIn* create(float d);

    virtual void startWithTarget(Node *target) override;

    virtual FadeIn* clone() const override;
    
    virtual FadeTo* reverse() const override;

    void setReverseAction(FadeTo* ac);

protected:

    FadeIn():_reverseAction(nullptr) {}

    virtual ~FadeIn() {}

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(FadeIn);

    FadeTo* _reverseAction;
};

class FadeOut : public FadeTo
{
public:

    static FadeOut* create(float d);

    virtual void startWithTarget(Node *target) override;

    virtual FadeOut* clone() const  override;

    virtual FadeTo* reverse() const override;

    void setReverseAction(FadeTo* ac);

protected:

    FadeOut():_reverseAction(nullptr) {}

    virtual ~FadeOut() {}

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(FadeOut);

    FadeTo* _reverseAction;
};

class TintTo : public ActionInterval
{
public:

    static TintTo* create(float duration, uint8_t red, uint8_t green, uint8_t blue);

    static TintTo* create(float duration, const DTRGB& color);

    virtual TintTo* clone() const override;

    virtual TintTo* reverse() const override;

    virtual void startWithTarget(Node *target) override;

    virtual void update(float time) override;
    
protected:

    TintTo() {}

    virtual ~TintTo() {}

    bool initWithDuration(float duration, uint8_t red, uint8_t green, uint8_t blue);

protected:

    DTRGB _to;

    DTRGB _from;

private:
    
    //CC_DISALLOW_COPY_AND_ASSIGN(TintTo);

};

class TintBy : public ActionInterval
{
public:

    static TintBy* create(float duration, int16_t deltaRed, int16_t deltaGreen, int16_t deltaBlue);

    virtual TintBy* clone() const override;

    virtual TintBy* reverse() const override;

    virtual void startWithTarget(Node *target) override;

    virtual void update(float time) override;
    
protected:

    TintBy() {}

    virtual ~TintBy() {}

    bool initWithDuration(float duration, int16_t deltaRed, int16_t deltaGreen, int16_t deltaBlue);

protected:
    int16_t _deltaR;
    int16_t _deltaG;
    int16_t _deltaB;

    int16_t _fromR;
    int16_t _fromG;
    int16_t _fromB;

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(TintBy);

};

class DelayTime : public ActionInterval
{
public:

    static DelayTime* create(float d);

    virtual void update(float time) override;

    virtual DelayTime* reverse() const override;

    virtual DelayTime* clone() const override;

protected:

    DelayTime() {}

    virtual ~DelayTime() {}

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(DelayTime);

};

class ReverseTime : public ActionInterval
{
public:

    static ReverseTime* create(FiniteTimeAction *action);

    virtual ReverseTime* reverse() const override;

    virtual ReverseTime* clone() const override;

    virtual void startWithTarget(Node *target) override;

    virtual void stop() override;

    virtual void update(float time) override;
    
protected:

    ReverseTime();

    virtual ~ReverseTime();

    bool initWithAction(FiniteTimeAction *action);

protected:

    FiniteTimeAction *_other;

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(ReverseTime);

};

class TargetedAction : public ActionInterval
{
public:

    static TargetedAction* create(Node* target, FiniteTimeAction* action);

    void setForcedTarget(Node* forcedTarget);

    Node* getForcedTarget() { return _forcedTarget; }

    const Node* getForcedTarget() const { return _forcedTarget; }

    virtual TargetedAction* clone() const override;

    virtual TargetedAction* reverse() const  override;

    virtual void startWithTarget(Node *target) override;

    virtual void stop() override;

    virtual void update(float time) override;
    
protected:

    TargetedAction();

    virtual ~TargetedAction();

    bool initWithTarget(Node* target, FiniteTimeAction* action);

protected:

    FiniteTimeAction* _action;

    Node* _forcedTarget;

private:

    //CC_DISALLOW_COPY_AND_ASSIGN(TargetedAction);

};

class ActionFloat : public ActionInterval
{
public:

    typedef std::function<void(float value)> ActionFloatCallback;

    static ActionFloat* create(float duration, float from, float to, ActionFloatCallback callback);

    void startWithTarget(Node* target) override;

    void update(float delta) override;

    ActionFloat* reverse() const override;

    ActionFloat* clone() const override;

protected:

    ActionFloat() {};

    virtual ~ActionFloat() {};

    bool initWithDuration(float duration, float from, float to, ActionFloatCallback callback);

protected:

    float _from;

    float _to;

    float _delta;

    ActionFloatCallback _callback;
private:

    //CC_DISALLOW_COPY_AND_ASSIGN(ActionFloat);

};

NS_DT_END


#endif //__DT_ACTION_INTERVAL_H__