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

#include "dtNode.h"
#include "base/dtScheduler.h"
#include "dtActionManager.h"
#include "base/dtEventDispatcher.h"
#include "base/dtMacros.h"
#include "dtDirector.h"
#include <limits.h> 
NS_DT_BEGIN



int Node::__attachedNodeCount = 0;

std::uint32_t Node::s_globalOrderOfArrival = 0;

Node::Node()
: _localZOrder(0)
, _position(0,0)
, _anchorPoint(0,0)
, _contentSize(0,0)
, _transform(Transform::IDENTITY)
, _parent(nullptr)
, _tag(Node::INVALID_TAG)
, _running(false)
, _visible(true)
{
    _director = Director::getInstance();
    _actionManager = _director->getActionManager();
    _actionManager->retain();
    _scheduler = _director->getScheduler();
    _scheduler->retain();
    _eventDispatcher = _director->getEventDispatcher();
    _eventDispatcher->retain();
}

Node::~Node()
{
    for (auto& child : _children)
    {
        child->_parent = nullptr;
    }
    
    stopAllActions();
    unscheduleAllCallbacks();
    DT_SAFE_RELEASE_NULL(_actionManager);
    DT_SAFE_RELEASE_NULL(_scheduler);

    _eventDispatcher->removeEventListenersForTarget(this);
    DT_SAFE_RELEASE(_eventDispatcher);
}

bool Node::init()
{
    return true;
}

Node * Node::create()
{
    Node * ret = new Node();
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

int Node::getAttachedNodeCount()
{
    return __attachedNodeCount;
}

void Node::setLocalZOrder(std::int32_t z)
{
    if (getLocalZOrder() == z)
        return;
    
    _setLocalZOrder(z);
    if (_parent)
    {
        _parent->reorderChild(this, z);
    }
    _eventDispatcher->setDirtyForNode(this);
}

void Node::updateOrderOfArrival()
{
    _orderOfArrival = (++s_globalOrderOfArrival);
}

void Node::setPosition(const Vec2& position)
{
    setPosition(position.x, position.y);
}

const Vec2& Node::getPosition() const
{
    return _position;
}

void Node::setPosition(int32_t x, int32_t y)
{
    if (_position.x == x && _position.y == y)
        return;
    _position.x = x;
    _position.y = y;
}

void Node::getPosition(int32_t* x, int32_t* y) const
{
    *x = _position.x;
    *y = _position.y;
}

void Node::setPositionX(int32_t x)
{
    setPosition(x, _position.y);
}

int32_t Node::getPositionX() const
{
    return _position.x;
}

void Node::setPositionY(int32_t y)
{
    setPosition(_position.x, y);
}

int32_t Node::getPositionY() const
{
    return  _position.y;
}

void Node::setAnchorPoint(const Vec2& point)
{
    if (! point.equals(_anchorPoint))
    {
        _anchorPoint = point;
    }
}

void Node::setAnchorPoint(int32_t x, int32_t y)
{
    Vec2 point(x,y);
    setAnchorPoint(point);
}

const Vec2& Node::getAnchorPoint() const
{
    return _anchorPoint;
}

void Node::setContentSize(const Size & size)
{
    if (! size.equals(_contentSize))
    {
        _contentSize = size;
    }
}

const Size& Node::getContentSize() const
{
    return _contentSize;
}

void Node::setVisible(bool visible)
{
    if(visible != _visible)
    {
        _visible = visible;
        if(_visible)
        {
            //_transformUpdated = _transformDirty = _inverseDirty = true;//TODO
        }
    }
}

bool Node::isVisible() const
{
    return _visible;
}

void Node::_setLocalZOrder(std::int32_t z)
{
    _localZOrder = z;
}

void Node::addChildHelper(Node* child, int localZOrder, int tag)
{
    auto assertNotSelfChild
        ( [ this, child ]() -> bool
          {
              for ( Node* parent( getParent() ); parent != nullptr;
                    parent = parent->getParent() )
                  if ( parent == child )
                      return false;
              
              return true;
          } );
    (void)assertNotSelfChild;
    
    if (_children.empty())
    {
        this->childrenAlloc();
    }
    
    this->insertChild(child, localZOrder);
    
    child->setTag(tag);
    
    child->setParent(this);

    child->updateOrderOfArrival();

    if( _running )
    {
        child->onEnter();
    }
}

void Node::addChild(Node *child)
{
    this->addChild(child, child->getLocalZOrder());
}

void Node::addChild(Node *child, int zOrder)
{
    this->addChild(child, zOrder, INVALID_TAG);
}

void Node::addChild(Node *child, int localZOrder, int tag)
{    
    addChildHelper(child, localZOrder, tag);
}

Node* Node::getChildByTag(int tag) const
{
    for (const auto child : _children)
    {
        if(child && child->_tag == tag)
            return child;
    }
    return nullptr;
}

ssize_t Node::getChildrenCount() const
{
    return _children.size();
}

void Node::setParent(Node * parent)
{
    _parent = parent;
    //_transformUpdated = _transformDirty = _inverseDirty = true;
}

void Node::removeFromParent()
{
    this->removeFromParentAndCleanup(true);
}

void Node::removeFromParentAndCleanup(bool cleanup)
{
    if (_parent != nullptr)
    {
        _parent->removeChild(this,cleanup);
    } 
}

void Node::removeChild(Node* child, bool cleanup /* = true */)
{
    // explicit nil handling
    if (_children.empty())
    {
        return;
    }

    ssize_t index = _children.getIndex(child);
    if( index != DT_INVALID_INDEX )
        this->detachChild( child, index, cleanup );
}

void Node::removeChildByTag(int tag, bool cleanup/* = true */)
{
    Node *child = this->getChildByTag(tag);

    if (child != nullptr)
    {
        this->removeChild(child, cleanup);
    }
    else
    {
        
    }
}

void Node::removeAllChildren()
{
    this->removeAllChildrenWithCleanup(true);
}

void Node::removeAllChildrenWithCleanup(bool cleanup)
{
    for (const auto& child : _children)
    {
        if(_running)
        {
            child->onExitTransitionDidStart();
            child->onExit();
        }

        if (cleanup)
        {
            child->cleanup();
        }
        child->setParent(nullptr);
    }
    
    _children.clear();
}

void Node::reorderChild(Node *child, int zOrder)
{
    //_reorderChildDirty = true;
    child->updateOrderOfArrival();
    child->_setLocalZOrder(zOrder);
}

void Node::sortAllChildren()
{
    // if (_reorderChildDirty) //TODO
    // {
        sortNodes(_children);
        // _reorderChildDirty = false;
        _eventDispatcher->setDirtyForNode(this);
    // }
}

int Node::getTag() const
{
    return _tag;
}

/// tag setter
void Node::setTag(int tag)
{
    _tag = tag ;
}

bool Node::isRunning() const
{
    return _running;
}

void Node::onEnter()
{
    if (!_running)
    {
        ++__attachedNodeCount;
    }

    for( const auto &child: _children)
        child->onEnter();
    
    this->resume();
    
    _running = true;
}


void Node::onEnterTransitionDidFinish()
{
    for( const auto &child: _children)
        child->onEnterTransitionDidFinish();
}

void Node::onExit()
{
    if (_running)
    {
        --__attachedNodeCount;
    }
    
    this->pause();
    
    _running = false;
    
    for( const auto &child: _children)
        child->onExit();
    
}

void Node::onExitTransitionDidStart()
{   
    for( const auto &child: _children)
        child->onExitTransitionDidStart();
}

void Node::cleanup()
{
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    for( const auto &child: _children)
        child->cleanup();
}

void Node::draw(Renderer*,const Transform& transform)
{

}

void Node::draw()
{
    // auto renderer = _director->getRenderer(); //TODO
    // draw(renderer, _modelViewTransform, FLAGS_TRANSFORM_DIRTY);
}

void Node::visit(Renderer* renderer,const Transform& transform)
{
    // quick return if not visible. children won't be drawn.
    if (!_visible)
    {
        return;
    }
    Transform parentTransform = this->transform(transform);
    int i = 0;
    if(!_children.empty())
    {
        sortAllChildren();
        for(auto size = _children.size(); i < size; ++i)
        {
            auto node = _children.at(i);

            if (node && node->_localZOrder < 0)
                node->visit(renderer, parentTransform);
            else
                break;
        }
        // self draw
        this->draw(renderer, parentTransform);
        for(auto it=_children.cbegin()+i, itCend = _children.cend(); it != itCend; ++it)
            (*it)->visit(renderer, parentTransform);
    }
    else
    {
        this->draw(renderer, parentTransform);
    }
}

void Node::visit()
{
    // auto renderer = _director->getRenderer(); //TODO
    // auto& parentTransform = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    // visit(renderer, parentTransform, FLAGS_TRANSFORM_DIRTY);
}

Scene* Node::getScene() const
{
    if (!_parent)
        return nullptr;
    
    auto sceneNode = _parent;
    while (sceneNode->_parent)
    {
        sceneNode = sceneNode->_parent;
    }
    if(sceneNode->nodeClassType()==1)
    {
        return (Scene*)(sceneNode);
    }else
    {
        return nullptr;
    }
    
}

void Node::setEventDispatcher(EventDispatcher* dispatcher)
{
    if (dispatcher != _eventDispatcher)
    {
        _eventDispatcher->removeEventListenersForTarget(this);
        DT_SAFE_RETAIN(dispatcher);
        DT_SAFE_RELEASE(_eventDispatcher);
        _eventDispatcher = dispatcher;
    }
}

void Node::setActionManager(ActionManager* actionManager)
{
    if( actionManager != _actionManager )
    {
        this->stopAllActions();
        DT_SAFE_RETAIN(actionManager);
        DT_SAFE_RELEASE(_actionManager);
        _actionManager = actionManager;
    }
}

Action * Node::runAction(Action* action)
{
    _actionManager->addAction(action, this, !_running);
    return action;
}

void Node::stopAllActions()
{
    _actionManager->removeAllActionsFromTarget(this);
}

void Node::stopAction(Action* action)
{
    _actionManager->removeAction(action);
}

void Node::stopActionByTag(int tag)
{
    _actionManager->removeActionByTag(tag, this);
}

void Node::stopAllActionsByTag(int tag)
{
    _actionManager->removeAllActionsByTag(tag, this);
}

void Node::stopActionsByFlags(unsigned int flags)
{
    if (flags > 0)
    {
        _actionManager->removeActionsByFlags(flags, this);
    }
}

Action * Node::getActionByTag(int tag)
{
    return _actionManager->getActionByTag(tag, this);
}

ssize_t Node::getNumberOfRunningActions() const
{
    return _actionManager->getNumberOfRunningActionsInTarget(this);
}

ssize_t Node::getNumberOfRunningActionsByTag(int tag) const
{
    return _actionManager->getNumberOfRunningActionsInTargetByTag(this, tag);
}

void Node::setScheduler(Scheduler* scheduler)
{
    if( scheduler != _scheduler )
    {
        this->unscheduleAllCallbacks();
        DT_SAFE_RETAIN(scheduler);
        DT_SAFE_RELEASE(_scheduler);
        _scheduler = scheduler;
    }
}

bool Node::isScheduled(SEL_SCHEDULE selector) const
{
    return _scheduler->isScheduled(selector, this);
}

bool Node::isScheduled(const std::string &key) const
{
    return _scheduler->isScheduled(key, this);
}

void Node::scheduleUpdate()
{
    scheduleUpdateWithPriority(0);
}

void Node::scheduleUpdateWithPriority(int priority)
{
    _scheduler->scheduleUpdate(this, priority, !_running);
}

void Node::unscheduleUpdate()
{
    _scheduler->unscheduleUpdate(this);
}

void Node::schedule(SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay)
{
    _scheduler->schedule(selector, this, interval , repeat, delay, !_running);
}

void Node::schedule(SEL_SCHEDULE selector, float interval)
{
    this->schedule(selector, interval, DT_REPEAT_FOREVER, 0.0f);
}

void Node::scheduleOnce(SEL_SCHEDULE selector, float delay)
{
    this->schedule(selector, 0.0f, 0, delay);
}

void Node::scheduleOnce(const std::function<void(float)> &callback, float delay, const std::string &key)
{
    _scheduler->schedule(callback, this, 0, 0, delay, !_running, key);
}

void Node::schedule(SEL_SCHEDULE selector)
{
    this->schedule(selector, 0.0f, DT_REPEAT_FOREVER, 0.0f);
}

void Node::schedule(const std::function<void(float)> &callback, const std::string &key)
{
    _scheduler->schedule(callback, this, 0, !_running, key);
}

void Node::schedule(const std::function<void(float)> &callback, float interval, const std::string &key)
{
    _scheduler->schedule(callback, this, interval, !_running, key);
}

void Node::schedule(const std::function<void(float)>& callback, float interval, unsigned int repeat, float delay, const std::string &key)
{
    _scheduler->schedule(callback, this, interval, repeat, delay, !_running, key);
}

void Node::unschedule(SEL_SCHEDULE selector)
{
    if (selector == nullptr)
        return;
    
    _scheduler->unschedule(selector, this);
}

void Node::unschedule(const std::string &key)
{
    _scheduler->unschedule(key, this);
}

void Node::unscheduleAllCallbacks()
{
    _scheduler->unscheduleAllForTarget(this);
}

void Node::resume()
{
    _scheduler->resumeTarget(this);
    _actionManager->resumeTarget(this);
    _eventDispatcher->resumeEventListenersForTarget(this);
}

void Node::pause()
{
    _scheduler->pauseTarget(this);
    _actionManager->pauseTarget(this);
    _eventDispatcher->pauseEventListenersForTarget(this);
}

void Node::update(float delta)
{

}

void Node::pauseEventDispatcher()
{
    _eventDispatcher->pauseEventListenersForTarget(this);
}

void Node::resumeEventDispatcher()
{
    _eventDispatcher->resumeEventListenersForTarget(this);
}

const Transform& Node::getNodeToParentTransform() const
{
    _transform.setTranslation(this->_position - this->_anchorPoint); //TODO dirty set
    return _transform;
}

Vec2 Node::convertToNodeSpace(const Vec2& worldPoint) const
{
    // Mat4 tmp = getWorldToNodeTransform(); //TODO
    // Vec3 vec3(worldPoint.x, worldPoint.y, 0);
    Vec2 ret;
    // tmp.transformPoint(Vec2,&ret);
    return Vec2(ret.x, ret.y);
}

Vec2 Node::convertToWorldSpace(const Vec2& nodePoint) const
{
    // Mat4 tmp = getNodeToWorldTransform(); //TODO
    // Vec3 vec3(nodePoint.x, nodePoint.y, 0);
    Vec2 ret;
    // tmp.transformPoint(vec3,&ret);
    return Vec2(ret.x, ret.y);

}

uint8_t Node::getOpacity(void) const
{
    return _realOpacity;
}

uint8_t Node::getDisplayedOpacity() const
{
    return _displayedOpacity;
}

void Node::setOpacity(uint8_t opacity)
{
    _displayedOpacity = _realOpacity = opacity;
    
    //updateCascadeOpacity(); //TODO
}

void Node::updateDisplayedOpacity(uint8_t parentOpacity)
{
    _displayedOpacity = _realOpacity * parentOpacity/255.0;
    // updateColor(); //TODO
    
    // if (_cascadeOpacityEnabled)
    // {
    //     for(const auto& child : _children)
    //     {
    //         child->updateDisplayedOpacity(_displayedOpacity);
    //     }
    // }
}

const DTRGB& Node::getColor(void) const
{
    return _realColor;
}

void Node::setColor(const DTRGB& color)
{
    _displayedColor = _realColor = color;
    
    // updateCascadeColor(); //TODO
}

void Node::childrenAlloc()
{
    _children.reserve(4);
}

uint32_t Node::processParentFlags(uint32_t parentFlags)
{
    //TODO
    uint32_t flags = parentFlags;

    return flags;
}

void Node::insertChild(Node* child, int z)
{
    _children.pushBack(child);
    child->_setLocalZOrder(z);
}

void Node::detachChild(Node *child, ssize_t childIndex, bool doCleanup)
{
    if (_running)
    {
        child->onExitTransitionDidStart();
        child->onExit();
    }
    if (doCleanup)
    {
        child->cleanup();
    }
    child->setParent(nullptr);
    _children.erase(childIndex);
}

Transform Node::transform(const Transform& parentTransform)
{
    return parentTransform.transform(Transform(this->_position - this->_anchorPoint));
}

NS_DT_END