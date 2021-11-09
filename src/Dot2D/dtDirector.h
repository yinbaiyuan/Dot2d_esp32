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

#ifndef __DT_DIRECTOR_H__
#define __DT_DIRECTOR_H__

#include "Print.h"

#include "base/dtMacros.h"
#include "base/dtRef.h"
#include "base/dtTypes.h"
#include "math/dtGeometry.h"
#include "base/dtScheduler.h"
#include "dtActionManager.h"
#include "base/dtEventDispatcher.h"
#include "dtScene.h"
#include "renderer/dtRenderer.h"

NS_DT_BEGIN

class DirectorDelegate : public Print
{
public:

    virtual void render() = 0;

    virtual size_t write(uint8_t) = 0;

    virtual void initMatrix(dot2d::DTRGB *data) = 0;

    virtual uint32_t dotOrder(uint16_t x,uint16_t y) = 0;

};

class Director : public Ref 
{
protected:

    Scheduler *_scheduler = nullptr;

    ActionManager *_actionManager = nullptr;

    EventDispatcher* _eventDispatcher = nullptr;

    Renderer *_renderer = nullptr;

    uint8_t _framesPerSecond = 0;

    uint8_t _oldFramesPerSecond = 0;

    Scene *_runningScene = nullptr;

    Scene *_nextScene = nullptr;

    bool _sendCleanupToScene = false;

    Vector<Scene*> _scenesStack;

    bool _paused = false;

    bool _invalid = false;

    bool _flushReady = true;

    int32_t _mainloopDelay = 0;

    bool _nextDeltaTimeZero = false;

    std::chrono::steady_clock::time_point _lastMainloop;

    std::chrono::steady_clock::time_point _lastUpdate;

private:

    uint32_t _frameInterval = 0;

    uint32_t _secondsPerFrame = 0;

    float _deltaTime = 0;

protected:

    DirectorDelegate *_directorDelegate = nullptr;

public:

    static Director* getInstance();

    Director();

    ~Director();
                  
    bool init();

    void setDelegate(DirectorDelegate *_delelate);

    void setFramesPerSecond(uint8_t f);

    uint8_t getFramesPerSecond(){ return _framesPerSecond; }

    uint32_t getFrameInterval(){ return _frameInterval; }

    uint32_t getSecondsPerFrame() { return _secondsPerFrame; } 

    void initDotCanvas(uint16_t width,uint16_t height);

    DTRGB* getDotCanvas();

    DTRGB* getDotCanvas(uint16_t* width,uint16_t* height);

    void clearDotCanvas(const DTRGB& color);

    const Size& getCanvasSize() const;

    uint16_t getDotCount();

    bool isSendCleanupToScene() { return _sendCleanupToScene; }

    Scene* getRunningScene() { return _runningScene; }

    bool isPaused() { return _paused; }

    bool isNextDeltaTimeZero() { return _nextDeltaTimeZero; }

    void setNextDeltaTimeZero(bool nextDeltaTimeZero);

    void runWithScene(Scene *scene);

    void pushScene(Scene *scene);

    void popScene();

    void popToRootScene();

    void popToSceneStackLevel(int8_t level);

    void replaceScene(Scene *scene);

    void end();

    void pause();

    void resume();

    void restart();

    void stopAnimation();

    void startAnimation();

    void mainLoop();

    void drawScene();

    Scheduler* getScheduler() const { return _scheduler; }   

    void setScheduler(Scheduler* scheduler);

    ActionManager* getActionManager() const { return _actionManager; }

    void setActionManager(ActionManager* actionManager);

    EventDispatcher* getEventDispatcher() const { return _eventDispatcher; }
    
    void setEventDispatcher(EventDispatcher* dispatcher);

    Renderer* getRenderer() const { return _renderer; }

    bool isValid() const { return !_invalid; }

    Print* loger(){return _directorDelegate;}

    void render();

protected:

    void reset();

    void purgeDirector();

    bool _purgeDirectorInNextLoop = false;
    
    void restartDirector();

    bool _restartDirectorInNextLoop = false;

    void setNextScene();

    void calculateDeltaTime();

};

NS_DT_END

#endif //__DT_DIRECTOR_H__