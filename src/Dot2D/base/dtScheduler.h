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

/****************************************************************************

Timer（定时器）是引擎定时执行任务的基础实现，Timer只用于处理单个定时任务。
Timer是一个虚类，定时执行的具体内容与定时器取消时执行的内容需要通过继承Timer类实现，
引擎给出了两个实现，请参考TimerTargetSelector和TimerTargetCallback。

Scheduler（调度器）是引擎的核心组件之一，管理全局生命周期中的Timer（定时器）。
同时，引擎中大部分延迟、定时执行的功能也是通过Scheduler调度的。

调度器可以设置两种定时任务：update selector 和 custom selector。
其中custom selector可以自定义定时执行的间隔时间，而update selector固定每帧调用一次，
但update selector拥有更好的性能和更少的内存占用。
****************************************************************************/

#ifndef __DT_SCHEDULER_H__
#define __DT_SCHEDULER_H__

#include <functional>
#include <mutex>
#include <set>

#include "dtRef.h"
#include "dtVector.h"
#include "uthash.h"

NS_DT_BEGIN

class Scheduler;

typedef std::function<void(float)> ccSchedulerFunc;

/**
 * @cond
 */

class Timer : public Ref
{
protected:

    /*对执行定时器的Scheduler的弱引用（不会增加引用计数），默认情况下只有一个全局的Scheduler*/
    Scheduler* _scheduler; // weak ref

    /*距离上一次定时任务执行过去的时间，单位为毫秒*/
    float _elapsed;

    /*是否重复无限次执行定时任务*/
    bool _runForever;

    /*是否有第一次执行的延迟时间*/
    bool _useDelay;

    /*定时任务的执行次数*/
    unsigned int _timesExecuted;

    /*需要重复执行的次数（不包括第一次执行的次数）*/
    unsigned int _repeat; //0 = once, 1 is 2 x executed

    /*第一次执行的延迟时间*/
    float _delay;

    /*定时执行的间隔时间，0代表每帧执行*/
    float _interval;

    /*定时器是否终止，已终止的定时器不能再次使用*/
    bool _aborted;

protected:
    
    Timer();

public:

    void setupTimerWithInterval(float seconds, unsigned int repeat, float delay);

    void setAborted() { _aborted = true; }

    bool isAborted() const { return _aborted; }

    /*判断定时器是否运行完毕*/
    bool isExhausted() const;
    
    /*定时执行的函数，dt为距离上次执行的毫秒间隔*/
    virtual void trigger(float dt) = 0;

    /*定时器执行结束时执行的函数*/
    virtual void cancel() = 0;
    
    /*由Scheduler每帧调用，根据距离上帧经过的毫秒时间调整定时任务的状态（执行定时任务、结束定时任务等）*/
    void update(float dt);

};

/*使用函数指针来记录需要定时调用的函数的定时器实现。
优势在于进行查找时可以直接根据函数指针进行查找，
但不支持c++11的lambda表达式。*/
class TimerTargetSelector : public Timer
{
public:
    
    TimerTargetSelector();

    bool initWithSelector(Scheduler* scheduler, SEL_SCHEDULE selector, Ref* target, float seconds, unsigned int repeat, float delay);
    
    SEL_SCHEDULE getSelector() const { return _selector; }
    
    virtual void trigger(float dt) override;
    
    virtual void cancel() override;

    virtual int8_t refClassType() override {return 11;}
    
protected:
    
    Ref* _target;
    
    SEL_SCHEDULE _selector;
};

/*为了支持lambda表达式而使用字符串作为查找标准的定时器实现。*/
class TimerTargetCallback : public Timer
{
public:

    TimerTargetCallback();
    
    bool initWithCallback(Scheduler* scheduler, const ccSchedulerFunc& callback, void *target, const std::string& key, float seconds, unsigned int repeat, float delay);
    
    const ccSchedulerFunc& getCallback() const { return _callback; }
    
    const std::string& getKey() const { return _key; }
    
    virtual void trigger(float dt) override;
    
    virtual void cancel() override;

    virtual int8_t refClassType() override{return 10;}
    
protected:
    
    void* _target;
    
    ccSchedulerFunc _callback;
    
    std::string _key;

};

/**
 * @endcond
 */

/**
 * @addtogroup base
 * @{
 */

struct _listEntry;

struct _hashSelectorEntry;

struct _hashUpdateEntry;

class Scheduler : public Ref
{
public:

    static const int PRIORITY_SYSTEM;
    
    static const int PRIORITY_NON_SYSTEM_MIN;
    
    Scheduler();
    
    virtual ~Scheduler();

    float getTimeScale() { return _timeScale; }

    void setTimeScale(float timeScale) { _timeScale = timeScale; }

    /*每帧对定时器进行调度。
    首先根据time scale将经过时间进行缩放，随后根据优先级从小到大调用所有的update，
    之后遍历调用所有的timer，调用完毕后对update过程中需要删除的update进行删除，
    随后调用所有的线程安全的函数回调。*/
    void update(float dt);

    /*新建一个自定义的定时器，如果已存在则更新信息。*/
    void schedule(const ccSchedulerFunc& callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& key);

    void schedule(const ccSchedulerFunc& callback, void *target, float interval, bool paused, const std::string& key);

    void schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused);

    void schedule(SEL_SCHEDULE selector, Ref *target, float interval, bool paused);

    template <class T>
    void scheduleUpdate(T *target, int priority, bool paused)
    {
        this->schedulePerFrame([target](float dt){
            target->update(dt);
        }, target, priority, paused);
    }

    /*移除一个自定义定时器，移除后如果节点目标没有任何定时器则将这个节点从hash表中移除。*/
    void unschedule(const std::string& key, void *target);

    void unschedule(SEL_SCHEDULE selector, Ref *target);

    void unscheduleUpdate(void *target);

    void unscheduleAllForTarget(void *target);

    void unscheduleAll();
    
    void unscheduleAllWithMinPriority(int minPriority);
    
    bool isScheduled(const std::string& key, const void *target) const;
    
    bool isScheduled(SEL_SCHEDULE selector, const Ref *target) const;
    
    void pauseTarget(void *target);

    void resumeTarget(void *target);

    bool isTargetPaused(void *target);

    std::set<void*> pauseAllTargets();

    std::set<void*> pauseAllTargetsWithMinPriority(int minPriority);

    void resumeTargets(const std::set<void*>& targetsToResume);

    void performFunctionInCocosThread(std::function<void()> function);

    void removeAllFunctionsToBePerformedInCocosThread();
    
protected:
    
    /*新建一个update定时器，如果已存在则更新优先级*/
    void schedulePerFrame(const ccSchedulerFunc& callback, void *target, int priority, bool paused);
    
    void removeHashElement(struct _hashSelectorEntry *element);

    /*移除一个update定时器，如果正处于update阶段则暂时不删除，而将节点加入待删除数组中等Scheduler的update末尾再进行删除*/
    void removeUpdateFromHash(struct _listEntry *entry);

    /*将一个update定时器根据优先级插入到指定的list中*/
    void priorityIn(struct _listEntry **list, const ccSchedulerFunc& callback, void *target, int priority, bool paused);
    
    /*将一个update定时器直接插入到指定的list中，用于优先级为0的列表*/
    void appendIn(struct _listEntry **list, const ccSchedulerFunc& callback, void *target, bool paused);

    /*时间缩放，影响所有的定时器（包括Action），值越大定时器执行的越快。*/
    float _timeScale;

    /*根据优先级分开存储的记录了定时update的双向链表，用于快速遍历update任务。*/
    struct _listEntry *_updatesNegList;        // list of priority < 0

    struct _listEntry *_updates0List;          // list priority == 0

    struct _listEntry *_updatesPosList;        // list priority > 0

    /*update/timer hash表的头节点，通过这个节点进行hash表的查询。*/
    struct _hashUpdateEntry *_hashForUpdates;
    
    struct _hashSelectorEntry *_hashForTimers;

    /*在Scheduler的update期间取消update定时器时，
    不会立刻移除而是放在此数组中，之后在Scheduler的update末尾再进行删除。
    如果立刻删除当前的update，就会导致无法根据next指针找到下一个update。*/
    std::vector<struct _listEntry *> _updateDeleteVector;

    /*指向当前正在执行定时任务的节点。*/
    struct _hashSelectorEntry *_currentTarget;
    
    /*当前节点时候不再需要，如果节点所有的定时器都被取消，节点会被从hash表中移除以提升性能。*/
    bool _currentTargetSalvaged;

    /*当前是否在Scheduler的update状态*/
    bool _updateHashLocked;
    
    /*需要在Scheduler的update末尾时调用的函数列表，添加和删除操作是线程安全的，一般用于在其他线程中调用主函数的回调。*/
    std::vector<std::function<void()>> _functionsToPerform;

    std::mutex _performMutex;

};

// end of base group
/** @} */

NS_DT_END

#endif //__DT_SCHEDULER_H__