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

#ifndef __DT_NODE_H__
#define __DT_NODE_H__

#include <cstdint>
#include "base/dtMacros.h"
#include "base/dtRef.h"
#include "base/dtVector.h"
#include "base/dtTypes.h"
#include "math/dtMath.h"
#include "math/dtGeometry.h"
#include "math/dtTransform.h"

NS_DT_BEGIN

class Renderer;
class Scene;
class ActionManager;
class EventDispatcher;
class Action;
class Scheduler;
class Director;

class Node : public Ref
{

protected:

    std::int32_t _localZOrder;

    std::uint32_t _orderOfArrival;

    float _globalZOrder;            ///< Global order used to sort the node

    static std::uint32_t s_globalOrderOfArrival;

    Vec2 _position;

    Vec2 _anchorPoint;  

    Size _contentSize; 

    mutable Transform _transform; 

    Vector<Node*> _children;

    Node *_parent;  

    Director* _director;  

    int _tag; 

    Scheduler *_scheduler;

    ActionManager *_actionManager;

    EventDispatcher* _eventDispatcher;

    bool _running;

    bool _visible;    

    static int __attachedNodeCount;

    uint8_t     _displayedOpacity;

    uint8_t     _realOpacity;

    DTRGB     _displayedColor;
    
    DTRGB     _realColor;

public:

    static const int INVALID_TAG = -1;

    Node();

    virtual ~Node();

    virtual bool init();

    static Node * create();

    static int getAttachedNodeCount();

    virtual int8_t nodeClassType(){return 0;}

public:

    virtual void setLocalZOrder(std::int32_t localZOrder);

    virtual std::int32_t getLocalZOrder() const { return _localZOrder; }

    void updateOrderOfArrival();

    virtual void setPosition(const Vec2 &position);

    virtual const Vec2& getPosition() const;

    virtual void setPosition(int32_t x, int32_t y);

    virtual void getPosition(int32_t* x, int32_t* y) const;

    virtual void  setPositionX(int32_t x);

    virtual int32_t getPositionX(void) const;

    virtual void  setPositionY(int32_t y);

    virtual int32_t getPositionY(void) const;

    virtual void setAnchorPoint(const Vec2& anchorPoint);

    virtual void setAnchorPoint(int32_t x, int32_t y);

    virtual const Vec2& getAnchorPoint() const;

    virtual void setContentSize(const Size& contentSize);

    virtual const Size& getContentSize() const;

    virtual void setVisible(bool visible);

    virtual bool isVisible() const;

private:

    virtual void _setLocalZOrder(std::int32_t z);

    void addChildHelper(Node* child, int localZOrder, int tag);

public:

    virtual void addChild(Node * child);

    virtual void addChild(Node * child, int localZOrder);

    virtual void addChild(Node* child, int localZOrder, int tag);

    virtual Node * getChildByTag(int tag) const;

    template <typename T>
    T getChildByTag(int tag) const { return static_cast<T>(getChildByTag(tag)); }

    virtual Vector<Node*>& getChildren() { return _children; }

    virtual const Vector<Node*>& getChildren() const { return _children; }

    virtual ssize_t getChildrenCount() const;

    virtual void setParent(Node* parent);

    virtual Node* getParent() { return _parent; }

    virtual const Node* getParent() const { return _parent; }

    virtual void removeFromParent();

    virtual void removeFromParentAndCleanup(bool cleanup);

    virtual void removeChild(Node* child, bool cleanup = true);

    virtual void removeChildByTag(int tag, bool cleanup = true);

    virtual void removeAllChildren();

    virtual void removeAllChildrenWithCleanup(bool cleanup);

    virtual void reorderChild(Node * child, int localZOrder);

    virtual void sortAllChildren();

    template<typename _T> inline
    static void sortNodes(Vector<_T*>& nodes)
    {
        std::sort(std::begin(nodes), std::end(nodes), [](_T* n1, _T* n2) {
            return (n1->_localZOrder == n2->_localZOrder && n1->_orderOfArrival < n2->_orderOfArrival) || n1->_localZOrder < n2->_localZOrder;
        });
    }

    virtual int getTag() const;

    virtual void setTag(int tag);

    virtual bool isRunning() const;

    virtual void onEnter();

    virtual void onEnterTransitionDidFinish();

    virtual void onExit();

    virtual void onExitTransitionDidStart();

    virtual void cleanup();

    virtual void draw(Renderer *renderer,const Transform& transform);

    virtual void draw() final;

    virtual void visit(Renderer *renderer,const Transform& transform);

    virtual void visit() final;

    virtual Scene* getScene() const;

    virtual void setEventDispatcher(EventDispatcher* dispatcher);

    virtual EventDispatcher* getEventDispatcher() const { return _eventDispatcher; };

    virtual void setActionManager(ActionManager* actionManager);

    virtual ActionManager* getActionManager() { return _actionManager; }

    virtual const ActionManager* getActionManager() const { return _actionManager; }

    virtual Action* runAction(Action* action);

    void stopAllActions();

    void stopAction(Action* action);

    void stopActionByTag(int tag);
    
    void stopAllActionsByTag(int tag);

    void stopActionsByFlags(unsigned int flags);

    Action* getActionByTag(int tag);

    ssize_t getNumberOfRunningActions() const;

    ssize_t getNumberOfRunningActionsByTag(int tag) const;

    virtual void setScheduler(Scheduler* scheduler);

    virtual Scheduler* getScheduler() { return _scheduler; }

    virtual const Scheduler* getScheduler() const { return _scheduler; }

    bool isScheduled(SEL_SCHEDULE selector) const;

    bool isScheduled(const std::string &key) const;

    void scheduleUpdate(void);

    void scheduleUpdateWithPriority(int priority);

    void unscheduleUpdate(void);

    void schedule(SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay);

    void schedule(SEL_SCHEDULE selector, float interval);

    void scheduleOnce(SEL_SCHEDULE selector, float delay);

    void scheduleOnce(const std::function<void(float)>& callback, float delay, const std::string &key);

    void schedule(SEL_SCHEDULE selector);

    void schedule(const std::function<void(float)>& callback, const std::string &key);

    void schedule(const std::function<void(float)>& callback, float interval, const std::string &key);

    void schedule(const std::function<void(float)>& callback, float interval, unsigned int repeat, float delay, const std::string &key);

    void unschedule(SEL_SCHEDULE selector);

    void unschedule(const std::string &key);

    void unscheduleAllCallbacks();

    virtual void resume(void);

    virtual void pause(void);

    virtual void update(float delta);

    virtual void pauseEventDispatcher();

    virtual void resumeEventDispatcher();

    virtual const Transform& getNodeToParentTransform() const;

    Vec2 convertToNodeSpace(const Vec2& worldPoint) const;

    Vec2 convertToWorldSpace(const Vec2& nodePoint) const;

    virtual uint8_t getOpacity() const;

    virtual uint8_t getDisplayedOpacity() const;

    virtual void setOpacity(uint8_t opacity);

    virtual void updateDisplayedOpacity(uint8_t parentOpacity);

    virtual const DTRGB& getColor() const;

    virtual void setColor(const DTRGB& color);

protected:

    void childrenAlloc(void);

    uint32_t processParentFlags(uint32_t parentFlags);

    void insertChild(Node* child, int z);

    void detachChild(Node *child, ssize_t index, bool doCleanup);

    Transform transform(const Transform& parentTransform);

};



NS_DT_END

#endif //__DT_NODE_H__