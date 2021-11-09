/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __DT_RANDOM_H_
#define __DT_RANDOM_H_

#include <random>
#include <cstdlib>
#include "dtMacros.h"

/**
* @addtogroup base
* @{
*/
NS_DT_BEGIN

class  RandomHelper {
public:
    template<typename T>
    static T random_real(T min, T max) {
        std::uniform_real_distribution<T> dist(min, max);
        auto &mt = RandomHelper::getEngine();
        return dist(mt);
    }

    template<typename T>
    static T random_int(T min, T max) {
        std::uniform_int_distribution<T> dist(min, max);
        auto &mt = RandomHelper::getEngine();
        return dist(mt);
    }
private:
    static std::mt19937 &getEngine();
};

template<typename T>
inline T random(T min, T max) {
    return RandomHelper::random_int<T>(min, max);
}

template<>
inline float random(float min, float max) {
    return RandomHelper::random_real(min, max);
}

template<>
inline long double random(long double min, long double max) {
    return RandomHelper::random_real(min, max);
}

template<>
inline double random(double min, double max) {
    return RandomHelper::random_real(min, max);
}

inline int random() {
    return dot2d::random(0, RAND_MAX);
};

inline float rand_minus1_1() {
    return ((std::rand() / (float)RAND_MAX) * 2) -1;
};

inline float rand_0_1() {
    return std::rand() / (float)RAND_MAX;
};

NS_DT_END
// end group
/// @}

#endif //__DT_RANDOM_H_