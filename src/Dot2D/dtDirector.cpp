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

#include "dtDirector.h"
#include "base/dtAutoreleasePool.h"

NS_DT_BEGIN


static Director* s_Director = nullptr;

Director* Director::getInstance()
{
    if (s_Director == nullptr)
    {
        s_Director = new Director();
        s_Director->init();
    }
    return s_Director;
}

Director::Director()
{
    
}

Director::~Director()
{
    delete _renderer;
    DT_SAFE_RELEASE(_eventDispatcher);
    DT_SAFE_RELEASE(_runningScene);
    DT_SAFE_RELEASE(_scheduler);
    DT_SAFE_RELEASE(_actionManager);
    s_Director = nullptr;
}

bool Director::init()
{
    _lastUpdate = std::chrono::steady_clock::now();
    
    _lastMainloop = std::chrono::steady_clock::now();

    _scenesStack.reserve(5);

    _scheduler = new Scheduler();

    _actionManager = new ActionManager();

    _scheduler->scheduleUpdate(_actionManager, Scheduler::PRIORITY_SYSTEM, false);

    _eventDispatcher = new EventDispatcher();

    _renderer = new Renderer();

    if (_eventDispatcher)
    {
        _eventDispatcher->setEnabled(true);
    }

    return true;
}

void Director::setDelegate(DirectorDelegate *_delelate)
{
    _directorDelegate = _delelate;
}

void Director::setFramesPerSecond(uint8_t f)
{
    _framesPerSecond = f;
    _frameInterval = 1000 / f;
}

void Director::initDotCanvas(uint16_t width,uint16_t height)
{
    _renderer->initDotCanvas(width,height);
    _directorDelegate->initMatrix(getDotCanvas());
    for (uint16_t y = 0; y < height; y++)
    {
        for (uint16_t x = 0; x < width; x++)
        {   
            _renderer->setMatrixOrder(x,y,_directorDelegate->dotOrder(x,y));
        }
    }
}

DTRGB* Director::getDotCanvas()
{
    return _renderer->getDotCanvas(); 
}

DTRGB* Director::getDotCanvas(uint16_t* width,uint16_t* height)
{
    return _renderer->getDotCanvas(width,height);
}

void Director::setNextDeltaTimeZero(bool nextDeltaTimeZero)
{
    _nextDeltaTimeZero = nextDeltaTimeZero;
}

const Size& Director::getCanvasSize() const
{
    return _renderer->getCanvasSize();
}

uint16_t Director::getDotCount()
{
    return _renderer->getDotCount();
}

void Director::clearDotCanvas(const DTRGB& color)
{
    _renderer->clearDotCanvas(color);
}

void Director::runWithScene(Scene *scene)
{
    pushScene(scene);
    startAnimation();
}

void Director::pushScene(Scene *scene)
{
    _sendCleanupToScene = false;

    _scenesStack.pushBack(scene);

    _nextScene = scene;
}

void Director::popScene()
{
    _scenesStack.popBack();

    ssize_t c = _scenesStack.size();

    if (c == 0)
    {
        end();
    }
    else
    {
        _sendCleanupToScene = true;
        _nextScene = _scenesStack.at(c - 1);
    }
}

void Director::popToRootScene()
{
    this->popToSceneStackLevel(1);
}

void Director::popToSceneStackLevel(int8_t level)
{
    ssize_t c = _scenesStack.size();

    if (level == 0)
    {
        end();
        return;
    }

    if (level >= c)
        return;

    auto firstOnStackScene = _scenesStack.back();
    if (firstOnStackScene == _runningScene)
    {
        _scenesStack.popBack();
        --c;
    }

    while (c > level)
    {
        auto current = _scenesStack.back();

        if (current->isRunning())
        {
            current->onExit();
        }

        current->cleanup();

        _scenesStack.popBack();

        --c;
    }

    _nextScene = _scenesStack.back();

    _sendCleanupToScene = true;
}

void Director::replaceScene(Scene *scene)
{
    if (_runningScene == nullptr) {
        runWithScene(scene);
        return;
    }
    
    if (scene == _nextScene)
    {
        return;
    }
    
    if (_nextScene)
    {
        if (_nextScene->isRunning())
        {
            _nextScene->onExit();
        }
        _nextScene->cleanup();

        _nextScene = nullptr;
    }

    ssize_t index = _scenesStack.size() - 1;

    _sendCleanupToScene = true;

    _scenesStack.replace(index, scene);

    _nextScene = scene;
}

void Director::end()
{
    _purgeDirectorInNextLoop = true;
}

void Director::pause()
{
    if (_paused)
    {
        return;
    }

    _oldFramesPerSecond = _framesPerSecond;

    this->setFramesPerSecond(4);

    _paused = true;
}

void Director::resume()
{
    if (! _paused)
    {
        return;
    }

    this->setFramesPerSecond(_oldFramesPerSecond);

    _paused = false;

    _deltaTime = 0;

    setNextDeltaTimeZero(true);
}

void Director::restart()
{
    _restartDirectorInNextLoop = true;
}

void Director::stopAnimation()
{
    _invalid = true;
}

void Director::startAnimation()
{
    _invalid = false;

    setNextDeltaTimeZero(true);
}

void Director::mainLoop()
{
    // if (_purgeDirectorInNextLoop)
    // {
    //     _purgeDirectorInNextLoop = false;
    //     purgeDirector();
    // }
    // else if (_restartDirectorInNextLoop)
    // {
    //     _restartDirectorInNextLoop = false;
    //     restartDirector();
    // }
    // else 
    if (! _invalid && _flushReady)
    {
        drawScene();

        AutoreleasePool::getInstance()->clear();

        auto now = std::chrono::steady_clock::now();

        int64_t dt = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastUpdate).count();
        
        _mainloopDelay = int32_t(_frameInterval * 1000 - dt);

        _flushReady = false;
    }

    auto now = std::chrono::steady_clock::now();
    int64_t mainloopDt = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastMainloop).count();
    _mainloopDelay -= mainloopDt;
    if(_mainloopDelay<=0)
    {
        _flushReady = true;
    }
    _lastMainloop = now;
    
}

void Director::drawScene()
{

    calculateDeltaTime();

    if (! _paused)
    {
        _scheduler->update(_deltaTime);
    }

    _renderer->clear();
    
    if (_nextScene)
    {
        setNextScene();
    }

    if (_runningScene)
    {
       _runningScene->render(_renderer,Transform::IDENTITY);
    }
    
    _renderer->render();

}

void Director::render()
{
    _directorDelegate->render();    
}

void Director::setScheduler(Scheduler* scheduler)
{
    if (_scheduler != scheduler)
    {
        DT_SAFE_RETAIN(scheduler);
        DT_SAFE_RELEASE(_scheduler);
        _scheduler = scheduler;
    }
}

void Director::setActionManager(ActionManager* actionManager)
{
    if (_actionManager != actionManager)
    {
        DT_SAFE_RETAIN(actionManager);
        DT_SAFE_RELEASE(_actionManager);
        _actionManager = actionManager;
    }   
}

void Director::setEventDispatcher(EventDispatcher* dispatcher)
{
    if (_eventDispatcher != dispatcher)
    {
        DT_SAFE_RETAIN(dispatcher);
        DT_SAFE_RELEASE(_eventDispatcher);
        _eventDispatcher = dispatcher;
    }
}

void Director::reset()
{
    if (_runningScene)
    {
        _runningScene->onExit();
        _runningScene->cleanup();
        _runningScene->release();
    }
    
    _runningScene = nullptr;
    _nextScene = nullptr;

    getScheduler()->unscheduleAll();

    if (_eventDispatcher)
    {
        _eventDispatcher->removeAllEventListeners();
    }

    _scenesStack.clear();
    
    stopAnimation();
}

void Director::purgeDirector()
{
    reset();

    release();
}

void Director::restartDirector()
{
    reset();
    
    getScheduler()->scheduleUpdate(getActionManager(), Scheduler::PRIORITY_SYSTEM, false);
    
    AutoreleasePool::getInstance()->clear();

    startAnimation();
}

void Director::setNextScene()
{
    if (_runningScene)
    {
        _runningScene->onExit();
    }

    if (_sendCleanupToScene && _runningScene)
    {
        _runningScene->cleanup();
    }

    if (_runningScene)
    {
        _runningScene->release();
    }
    _runningScene = _nextScene;
    _nextScene->retain();
    _nextScene = nullptr;

    if (_runningScene)
    {
        _runningScene->onEnter();
    }
}

void Director::calculateDeltaTime()
{
    if (_nextDeltaTimeZero)
    {
        _deltaTime = 0;
        _nextDeltaTimeZero = false;
        _lastUpdate = std::chrono::steady_clock::now();
    }
    else
    {
        auto now = std::chrono::steady_clock::now();
        _deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastUpdate).count() / 1000000.0f;
        _lastUpdate = now;
        _deltaTime = MAX(0, _deltaTime);
    }
}

NS_DT_END
