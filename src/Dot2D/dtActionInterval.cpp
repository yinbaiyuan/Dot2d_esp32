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

#include "dtActionInterval.h"
#include "dtActionInstant.h"
#include "dtNode.h"

NS_DT_BEGIN

class ExtraAction : public FiniteTimeAction
{
public:

    static ExtraAction* create();

    virtual ExtraAction* clone() const;

    virtual ExtraAction* reverse() const;

    virtual void update(float time);

    virtual void step(float dt);

};

ExtraAction* ExtraAction::create()
{

    ExtraAction* ret = new ExtraAction();
    if (ret)
    {
        ret->autorelease();
    }
    return ret;

}

ExtraAction* ExtraAction::clone() const
{
    return ExtraAction::create();
}

ExtraAction* ExtraAction::reverse() const
{
    return ExtraAction::create();
}

void ExtraAction::update(float /*time*/)
{

}

void ExtraAction::step(float /*dt*/)
{

}

bool ActionInterval::initWithDuration(float d)
{
    _duration = std::abs(d) <= MATH_EPSILON ? MATH_EPSILON : d;
    _elapsed = 0;
    _firstTick = true;
    _done = false;
    
    return true;
}

float ActionInterval::getAmplitudeRate()
{
    return 0;
}

bool ActionInterval::isDone() const
{
    return _done;
}

void ActionInterval::step(float dt)
{
    if (_firstTick)
    {
        _firstTick = false;
        _elapsed = MATH_EPSILON;
    }
    else
    {
        _elapsed += dt;
    }
    
    
    float updateDt = std::max(0.0f,                                  // needed for rewind. elapsed could be negative
                              std::min(1.0f, _elapsed / _duration)
                              );

    
    this->update(updateDt);

    _done = _elapsed >= _duration;

}

void ActionInterval::startWithTarget(Node *target)
{
    FiniteTimeAction::startWithTarget(target);
    _elapsed = 0.0f;
    _firstTick = true;
    _done = false;
}

Sequence* Sequence::create(FiniteTimeAction *action1, ...)
{
    va_list params;
    va_start(params, action1);
    Sequence *ret = Sequence::createWithVariableList(action1, params);
    va_end(params);
    return ret;
}

Sequence* Sequence::create(const Vector<FiniteTimeAction*>& arrayOfActions)
{
    Sequence* seq = new  Sequence;
    if (seq && seq->init(arrayOfActions))
    {
        seq->autorelease();
        return seq;
    }
    delete seq;
    return nullptr;
}

Sequence* Sequence::createWithVariableList(FiniteTimeAction *action1, va_list args)
{
    FiniteTimeAction *now;
    FiniteTimeAction *prev = action1;
    bool bOneAction = true;
    while (action1)
    {
        now = va_arg(args, FiniteTimeAction*);
        if (now)
        {
            prev = createWithTwoActions(prev, now);
            bOneAction = false;
        }
        else
        {
            if (bOneAction)
            {
                prev = createWithTwoActions(prev, ExtraAction::create());
            }
            break;
        }
    }
    return ((Sequence*)prev);
}

Sequence* Sequence::createWithTwoActions(FiniteTimeAction *actionOne, FiniteTimeAction *actionTwo)
{
    Sequence *sequence = new Sequence();
    if (sequence && sequence->initWithTwoActions(actionOne, actionTwo))
    {
        sequence->autorelease();
        return sequence;
    }
    delete sequence;
    return nullptr;
}

Sequence* Sequence::clone() const
{
    if (_actions[0] && _actions[1])
    {
        return Sequence::create(_actions[0]->clone(), _actions[1]->clone(), nullptr);
    } else {
        return nullptr;
    }
}

Sequence* Sequence::reverse() const
{
    if (_actions[0] && _actions[1])
        return Sequence::createWithTwoActions(_actions[1]->reverse(), _actions[0]->reverse());
    else
        return nullptr;
}

void Sequence::startWithTarget(Node *target)
{
    if (target == nullptr){return;}
    if (_actions[0] == nullptr || _actions[1] == nullptr){return;}
    if (_duration > FLT_EPSILON)
    {
        _split = _actions[0]->getDuration() > FLT_EPSILON ? _actions[0]->getDuration() / _duration : 0;
    }
    ActionInterval::startWithTarget(target);
    _last = -1;
}

void Sequence::stop()
{
    if( _last != - 1 && _actions[_last])
    {
        _actions[_last]->stop();
    }
    ActionInterval::stop();
}

bool Sequence::isDone() const
{
    
    if (_actions[1] && _actions[1]->actionClassType()==1)
    {
        return (_done && _actions[1]->isDone());
    }
    else
    {
        return _done;
    }
}

void Sequence::update(float t)
{
    int found = 0;
    float new_t = 0.0f;

    if( t < _split )
    {
        found = 0;
        if( _split != 0 )
            new_t = t / _split;
        else
            new_t = 1;
    }
    else
    {
        found = 1;
        if ( _split == 1 )
            new_t = 1;
        else
            new_t = (t-_split) / (1 - _split );
    }
    if ( found==1 )
    {
        if( _last == -1 )
        {
            _actions[0]->startWithTarget(_target);
            _actions[0]->update(1.0f);
            _actions[0]->stop();
        }
        else if( _last == 0 )
        {
            _actions[0]->update(1.0f);  
            _actions[0]->stop();
        }
    }
    else if(found==0 && _last==1 )
    {
        _actions[1]->update(0);    
        _actions[1]->stop();
    }
    if( found == _last && _actions[found]->isDone() )
    {
        return;
    }
    if( found != _last )
    {
        _actions[found]->startWithTarget(_target);
    }
     _actions[found]->update(new_t);   
    _last = found;
}

Sequence::Sequence()
: _split(0)
{
    _actions[0] = nullptr;
    _actions[1] = nullptr;
}

Sequence::~Sequence()
{
    DT_SAFE_RELEASE(_actions[0]);
    DT_SAFE_RELEASE(_actions[1]);
}

bool Sequence::initWithTwoActions(FiniteTimeAction *actionOne, FiniteTimeAction *actionTwo)
{
    if (actionOne == nullptr || actionTwo == nullptr)
    {
        return false;
    }
    float d = actionOne->getDuration() + actionTwo->getDuration();
    ActionInterval::initWithDuration(d);
    _actions[0] = actionOne;
    actionOne->retain();
    _actions[1] = actionTwo;
    actionTwo->retain();
    return true;
}

bool Sequence::init(const Vector<FiniteTimeAction*>& arrayOfActions)
{
    auto count = arrayOfActions.size();
    if (count == 0)
        return false;

    if (count == 1)
        return initWithTwoActions(arrayOfActions.at(0), ExtraAction::create());

    auto prev = arrayOfActions.at(0);
    for (int i = 1; i < count-1; ++i)
    {
        prev = createWithTwoActions(prev, arrayOfActions.at(i));
    }
    return initWithTwoActions(prev, arrayOfActions.at(count-1));
}


Repeat* Repeat::create(FiniteTimeAction *action, unsigned int times)
{
    Repeat* repeat = new Repeat();
    if (repeat && repeat->initWithAction(action, times))
    {
        repeat->autorelease();
        return repeat;
    }
    delete repeat;
    return nullptr;
}

Repeat* Repeat::clone() const
{
    return Repeat::create(_innerAction->clone(), _times);
}

Repeat* Repeat::reverse() const
{
    return Repeat::create(_innerAction->reverse(), _times);
}

void Repeat::startWithTarget(Node *target)
{
    _total = 0;
    _nextDt = _innerAction->getDuration() / _duration;
    ActionInterval::startWithTarget(target);
    _innerAction->startWithTarget(target);
}

void Repeat::stop()
{
    _innerAction->stop();
    ActionInterval::stop();
}

void Repeat::update(float dt)
{
    if (dt >= _nextDt)
    {
        while (dt >= _nextDt && _total < _times)
        {
            _innerAction->update(1.0f);
            _total++;

            _innerAction->stop();
            _innerAction->startWithTarget(_target);
            _nextDt = _innerAction->getDuration()/_duration * (_total+1);
        }

        // fix for issue #1288, incorrect end value of repeat
        if (std::abs(dt - 1.0f) < FLT_EPSILON && _total < _times)
        {
            _innerAction->update(1.0f);
            _total++;
        }

        // don't set an instant action back or update it, it has no use because it has no duration
        if (!_actionInstant)
        {
            if (_total == _times)
            {
                // minggo: inner action update is invoked above, don't have to invoke it here
//                if (!(sendUpdateEventToScript(1, _innerAction)))
//                    _innerAction->update(1);
                _innerAction->stop();
            }
            else
            {
                // issue #390 prevent jerk, use right update
                _innerAction->update(dt - (_nextDt - _innerAction->getDuration()/_duration));
            }
        }
    }
    else
    {
        _innerAction->update(fmodf(dt * _times,1.0f));
    }
}

bool Repeat::isDone() const
{
    return _total == _times;
}

Repeat::~Repeat()
{
    DT_SAFE_RELEASE(_innerAction);
}

bool Repeat::initWithAction(FiniteTimeAction *action, unsigned int times)
{
    if (action && ActionInterval::initWithDuration(action->getDuration() * times))
    {
        _times = times;

        _innerAction = action;

        action->retain();

        _actionInstant = action->actionClassType()==1 ? true : false;

        _total = 0;

        return true;
    }
    return false;
}

RepeatForever *RepeatForever::create(ActionInterval *action)
{
    RepeatForever *ret = new RepeatForever();
    if (ret && ret->initWithAction(action))
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool RepeatForever::initWithAction(ActionInterval *action)
{
    if (action == nullptr)
    {
        return false;
    }
    
    action->retain();
    _innerAction = action;
    
    return true;
}

RepeatForever *RepeatForever::clone() const
{
    return RepeatForever::create(_innerAction->clone());
}

RepeatForever *RepeatForever::reverse() const
{
    return RepeatForever::create(_innerAction->reverse());
}

void RepeatForever::startWithTarget(Node* target)
{
    ActionInterval::startWithTarget(target);
    _innerAction->startWithTarget(target);
}

void RepeatForever::step(float dt)
{
    _innerAction->step(dt);

    if (_innerAction->isDone() && _innerAction->getDuration() > 0)
    {
        float diff = _innerAction->getElapsed() - _innerAction->getDuration();
        if (diff > _innerAction->getDuration())
            diff = fmodf(diff, _innerAction->getDuration());
        _innerAction->startWithTarget(_target);
        _innerAction->step(0.0f);
        _innerAction->step(diff);
        
    }
}

bool RepeatForever::isDone() const
{
    return false;
}

RepeatForever::~RepeatForever()
{
    DT_SAFE_RELEASE(_innerAction);
}

Spawn* Spawn::create(FiniteTimeAction *action1, ...)
{
    va_list params;
    va_start(params, action1);
    Spawn *ret = Spawn::createWithVariableList(action1, params);
    va_end(params);
    return ret;
}

Spawn* Spawn::createWithVariableList(FiniteTimeAction *action1, va_list args)
{
    FiniteTimeAction *now;
    FiniteTimeAction *prev = action1;
    bool oneAction = true;
    while (action1)
    {
        now = va_arg(args, FiniteTimeAction*);
        if (now)
        {
            prev = createWithTwoActions(prev, now);
            oneAction = false;
        }
        else
        {
            if (oneAction)
            {
                prev = createWithTwoActions(prev, ExtraAction::create());
            }
            break;
        }
    }
    return ((Spawn*)prev);
}

Spawn* Spawn::create(const Vector<FiniteTimeAction*>& arrayOfActions)
{
    Spawn* ret = new Spawn;
    if (ret && ret->init(arrayOfActions))
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Spawn* Spawn::createWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2)
{
    Spawn *spawn = new Spawn();
    if (spawn && spawn->initWithTwoActions(action1, action2))
    {
        spawn->autorelease();
        return spawn;
    }
    delete spawn;
    return nullptr;
}

Spawn* Spawn::clone() const
{
    if (_one && _two)
        return Spawn::createWithTwoActions(_one->clone(), _two->clone());
    else
        return nullptr;
}

Spawn* Spawn::reverse() const
{
    if (_one && _two)
        return Spawn::createWithTwoActions(_one->reverse(), _two->reverse());

    return nullptr;
}

void Spawn::startWithTarget(Node *target)
{
    if (target == nullptr)
    {
        return;
    }
    if (_one == nullptr || _two == nullptr)
    {
        return;
    }
    
    ActionInterval::startWithTarget(target);
    _one->startWithTarget(target);
    _two->startWithTarget(target);
}

void Spawn::stop()
{
    if (_one)
        _one->stop();

    if (_two)
        _two->stop();

    ActionInterval::stop();
}

void Spawn::update(float time)
{
    if (_one)
    {
        _one->update(time);  
    }
    if (_two)
    {
        _two->update(time);   
    }
}

Spawn::Spawn()
: _one(nullptr)
, _two(nullptr)
{
    
}

Spawn::~Spawn()
{
    DT_SAFE_RELEASE(_one);
    DT_SAFE_RELEASE(_two);
}

bool Spawn::initWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2)
{
    if (action1 == nullptr || action2 == nullptr)
    {
        return false;
    }

    bool ret = false;

    float d1 = action1->getDuration();
    float d2 = action2->getDuration();

    if (ActionInterval::initWithDuration(MAX(d1, d2)))
    {
        _one = action1;
        _two = action2;

        if (d1 > d2)
        {
            _two = Sequence::createWithTwoActions(action2, DelayTime::create(d1 - d2));
        } 
        else if (d1 < d2)
        {
            _one = Sequence::createWithTwoActions(action1, DelayTime::create(d2 - d1));
        }

        _one->retain();
        _two->retain();

        ret = true;
    }

    return ret;
}

bool Spawn::init(const Vector<FiniteTimeAction*>& arrayOfActions)
{
    auto count = arrayOfActions.size();
    if (count == 0)
        return false;
    
    if (count == 1)
        return initWithTwoActions(arrayOfActions.at(0), ExtraAction::create());
    
    auto prev = arrayOfActions.at(0);
    for (int i = 1; i < count-1; ++i)
    {
        prev = createWithTwoActions(prev, arrayOfActions.at(i));
    }
    return initWithTwoActions(prev, arrayOfActions.at(count-1));
}

MoveBy* MoveBy::create(float duration, const Vec2& deltaPosition)
{
    MoveBy *ret = new MoveBy();
    
    if (ret && ret->initWithDuration(duration, deltaPosition))
    {
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

MoveBy* MoveBy::clone() const
{
    return MoveBy::create(_duration, _positionDelta);
}

MoveBy* MoveBy::reverse() const
{
    return MoveBy::create(_duration, -_positionDelta);
}

void MoveBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousPosition = _startPosition = target->getPosition(); 
}

void MoveBy::update(float t)
{
    if (_target)
    {
        _target->setPosition(_startPosition + _positionDelta * t);
    }
}

bool MoveBy::initWithDuration(float duration, const Vec2& deltaPosition)
{
    bool ret = false;
    if (ActionInterval::initWithDuration(duration))
    {
        _positionDelta = deltaPosition;
        ret = true;
    }
    return ret;
}

MoveTo* MoveTo::create(float duration, const Vec2& position)
{
    MoveTo *ret = new MoveTo();
    if (ret && ret->initWithDuration(duration, position))
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

MoveTo* MoveTo::clone() const
{
    return MoveTo::create(_duration, _endPosition);
}

MoveTo* MoveTo::reverse() const
{
    return nullptr;
}

void MoveTo::startWithTarget(Node *target)
{
    MoveBy::startWithTarget(target);
    _positionDelta = _endPosition - target->getPosition();
}

bool MoveTo::initWithDuration(float duration, const Vec2& position)
{
    bool ret = false;
    if (ActionInterval::initWithDuration(duration))
    {
        _endPosition = position;
        ret = true;
    }
    return ret;
}

Blink* Blink::create(float duration, int blinks)
{
    Blink *blink = new Blink();
    if (blink && blink->initWithDuration(duration, blinks))
    {
        blink->autorelease();
        return blink;
    }
    delete blink;
    return nullptr;
}

Blink* Blink::clone() const
{
    return Blink::create(_duration, _times);
}

Blink* Blink::reverse() const
{
    return Blink::create(_duration, _times);
}

void Blink::update(float time)
{
    if (_target && ! isDone())
    {
        float slice = 1.0f / _times;
        float m = fmodf(time, slice);
        _target->setVisible(m > slice / 2 ? true : false); 
    }
}

void Blink::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _originalState = target->isVisible();
}

void Blink::stop()
{
    if (nullptr != _target)
        _target->setVisible(_originalState);
    ActionInterval::stop();
}

bool Blink::initWithDuration(float duration, int blinks)
{
    if (blinks < 0)
    {
        return false;
    }
    if (ActionInterval::initWithDuration(duration) && blinks>=0)
    {
        _times = blinks;
        return true;
    }
    return false;
}

FadeTo* FadeTo::create(float duration, uint8_t opacity)
{
    FadeTo *fadeTo = new FadeTo();
    if (fadeTo && fadeTo->initWithDuration(duration, opacity))
    {
        fadeTo->autorelease();
        return fadeTo;
    }
    delete fadeTo;
    return nullptr;
}

FadeTo* FadeTo::clone() const
{
    return FadeTo::create(_duration, _toOpacity);
}

FadeTo* FadeTo::reverse() const
{
    return nullptr;
}

void FadeTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    if (target)
    {
        _fromOpacity = target->getOpacity(); 
    }
}

void FadeTo::update(float time)
{
    if (_target)
    {
        _target->setOpacity((uint8_t)(_fromOpacity + (_toOpacity - _fromOpacity) * time));
    }
}

bool FadeTo::initWithDuration(float duration, uint8_t opacity)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _toOpacity = opacity;
        return true;
    }
    return false;
}

FadeIn* FadeIn::create(float d)
{
    FadeIn* action = new FadeIn();
    if (action && action->initWithDuration(d,255.0f))
    {
        action->autorelease();
        return action;
    }
    delete action;
    return nullptr;
}

void FadeIn::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    if (nullptr != _reverseAction)
        this->_toOpacity = this->_reverseAction->_fromOpacity;
    else
        _toOpacity = 255.0f;
    
    if (target)
    {
        _fromOpacity = target->getOpacity(); 
    }
}

FadeIn* FadeIn::clone() const
{
    return FadeIn::create(_duration);
}

FadeTo* FadeIn::reverse() const
{
    auto action = FadeOut::create(_duration);
    action->setReverseAction(const_cast<FadeIn*>(this));
    return action;
}

void FadeIn::setReverseAction(FadeTo *ac)
{
    _reverseAction = ac;
}

FadeOut* FadeOut::create(float d)
{
    FadeOut* action = new FadeOut();
    if (action && action->initWithDuration(d,0.0f))
    {
        action->autorelease();
        return action;
    }
    delete action;
    return nullptr;
}

void FadeOut::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    
    if (nullptr != _reverseAction)
        _toOpacity = _reverseAction->_fromOpacity;
    else
        _toOpacity = 0.0f;
    
    if (target)
    {
        _fromOpacity = target->getOpacity(); 
    }
}

FadeOut* FadeOut::clone() const
{
    return FadeOut::create(_duration);
}

FadeTo* FadeOut::reverse() const
{
    auto action = FadeIn::create(_duration);
    action->setReverseAction(const_cast<FadeOut*>(this));
    return action;
}

void FadeOut::setReverseAction(FadeTo *ac)
{
    _reverseAction = ac;
}

TintTo* TintTo::create(float duration, uint8_t red, uint8_t green, uint8_t blue)
{
    TintTo *tintTo = new TintTo();
    if (tintTo && tintTo->initWithDuration(duration, red, green, blue))
    {
        tintTo->autorelease();
        return tintTo;
    }
    delete tintTo;
    return nullptr;
}

TintTo* TintTo::create(float duration, const DTRGB& color)
{
    return create(duration, color.r, color.g, color.b);
}

TintTo* TintTo::clone() const
{
    return TintTo::create(_duration, _to.r, _to.g, _to.b);
}

TintTo* TintTo::reverse() const
{
    return nullptr;
}

void TintTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    if (_target)
    {
        _from = _target->getColor(); 
    }
}

void TintTo::update(float time)
{
    if (_target)
    {
        _target->setColor(DTRGB(uint8_t(_from.r + (_to.r - _from.r) * time),
            (uint8_t)(_from.g + (_to.g - _from.g) * time),
            (uint8_t)(_from.b + (_to.b - _from.b) * time)));
    }
}


bool TintTo::initWithDuration(float duration, uint8_t red, uint8_t green, uint8_t blue)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _to = DTRGB(red, green, blue);
        return true;
    }
    return false;
}

TintBy* TintBy::create(float duration, int16_t deltaRed, int16_t deltaGreen, int16_t deltaBlue)
{
    TintBy *tintBy = new TintBy();
    if (tintBy && tintBy->initWithDuration(duration, deltaRed, deltaGreen, deltaBlue))
    {
        tintBy->autorelease();
        return tintBy;
    }
    delete tintBy;
    return nullptr;
}

TintBy* TintBy::clone() const
{
    return TintBy::create(_duration, _deltaR, _deltaG, _deltaB);
}

TintBy* TintBy::reverse() const
{
    return TintBy::create(_duration, -_deltaR, -_deltaG, -_deltaB);
}

void TintBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    if (target)
    {
        DTRGB color = target->getColor(); 
        _fromR = color.r;
        _fromG = color.g;
        _fromB = color.b;
    }    
}

void TintBy::update(float time)
{
    if (_target)
    {
        _target->setColor(DTRGB((uint8_t)(_fromR + _deltaR * time),
            (uint8_t)(_fromG + _deltaG * time),
            (uint8_t)(_fromB + _deltaB * time))); 
    }    
}

bool TintBy::initWithDuration(float duration, int16_t deltaRed, int16_t deltaGreen, int16_t deltaBlue)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _deltaR = deltaRed;
        _deltaG = deltaGreen;
        _deltaB = deltaBlue;

        return true;
    }
    return false;
}

DelayTime* DelayTime::create(float d)
{
    DelayTime* action = new DelayTime();
    if (action && action->initWithDuration(d))
    {
        action->autorelease();
        return action;
    }
    
    delete action;
    return nullptr;
}

void DelayTime::update(float /*time*/)
{
    return;
}

DelayTime* DelayTime::reverse() const
{
    return DelayTime::create(_duration);
}

DelayTime* DelayTime::clone() const
{
    // no copy constructor
    return DelayTime::create(_duration);
}

ReverseTime* ReverseTime::create(FiniteTimeAction *action)
{
    // casting to prevent warnings
    ReverseTime *reverseTime = new ReverseTime();
    if (reverseTime && reverseTime->initWithAction( action->clone() ))
    {
        reverseTime->autorelease();
        return reverseTime;
    }
    delete reverseTime;
    return nullptr;
}

ReverseTime* ReverseTime::reverse() const
{
    return (ReverseTime*)_other->clone();
}

ReverseTime* ReverseTime::clone() const
{
    return ReverseTime::create(_other->clone());
}

void ReverseTime::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _other->startWithTarget(target);
}

void ReverseTime::stop()
{
    _other->stop();
    ActionInterval::stop();
}

void ReverseTime::update(float time)
{
    if (_other)
    {
        _other->update(1 - time);
    }
}

ReverseTime::ReverseTime() : _other(nullptr)
{

}

ReverseTime::~ReverseTime()
{
    DT_SAFE_RELEASE(_other);
}

bool ReverseTime::initWithAction(FiniteTimeAction *action)
{
    if (action == nullptr || action == _other)
    {
        return false;
    }

    if (ActionInterval::initWithDuration(action->getDuration()))
    {
        DT_SAFE_RELEASE(_other);
        _other = action;
        action->retain();

        return true;
    }

    return false;
}


TargetedAction::TargetedAction()
: _action(nullptr)
, _forcedTarget(nullptr)
{

}

TargetedAction::~TargetedAction()
{
    DT_SAFE_RELEASE(_forcedTarget);
    DT_SAFE_RELEASE(_action);
}

TargetedAction* TargetedAction::create(Node* target, FiniteTimeAction* action)
{
    TargetedAction* p = new TargetedAction();
    if (p && p->initWithTarget(target, action))
    {
        p->autorelease();
        return p;
    }
    delete p;
    return nullptr;
}

void TargetedAction::setForcedTarget(Node* forcedTarget)
{
    if( _forcedTarget != forcedTarget )
    {
        DT_SAFE_RETAIN(forcedTarget);
        DT_SAFE_RELEASE(_forcedTarget);
        _forcedTarget = forcedTarget;
    }
}

TargetedAction* TargetedAction::clone() const
{
    return TargetedAction::create(_forcedTarget, _action->clone());
}


TargetedAction* TargetedAction::reverse() const
{
    auto a = new TargetedAction();
    a->initWithTarget(_forcedTarget, _action->reverse());
    a->autorelease();
    return a;
}

void TargetedAction::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _action->startWithTarget(_forcedTarget);
}

void TargetedAction::stop()
{
    _action->stop();
}

void TargetedAction::update(float time)
{
    _action->update(time);
}

bool TargetedAction::initWithTarget(Node* target, FiniteTimeAction* action)
{
    if(ActionInterval::initWithDuration(action->getDuration()))
    {
        DT_SAFE_RETAIN(target);
        _forcedTarget = target;
        DT_SAFE_RETAIN(action);
        _action = action;
        return true;
    }
    return false;
}

ActionFloat* ActionFloat::create(float duration, float from, float to, ActionFloatCallback callback)
{
    auto ref = new ActionFloat();
    if (ref && ref->initWithDuration(duration, from, to, callback))
    {
        ref->autorelease();
        return ref;
    }
    delete ref;
    return nullptr;
}

void ActionFloat::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _delta = _to - _from;
}

void ActionFloat::update(float delta)
{
    float value = _to - _delta * (1 - delta);
    if (_callback)
    {
        _callback(value);
    }
}

ActionFloat* ActionFloat::reverse() const
{
    return ActionFloat::create(_duration, _to, _from, _callback);
}

ActionFloat* ActionFloat::clone() const
{
    return ActionFloat::create(_duration, _from, _to, _callback);
}

bool ActionFloat::initWithDuration(float duration, float from, float to, ActionFloatCallback callback)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _from = from;
        _to = to;
        _callback = callback;
        return true;
    }
    return false;
}

NS_DT_END
