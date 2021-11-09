/****************************************************************************
Copyright (c) 2021      Yinbaiyuan

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

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "OneButton.h"

enum BaseButtonType
{
    BBT_NONE    = -1,
    BBT_UP      = 0,
    BBT_RIGHT   = 1,
    BBT_DOWN    = 2,
    BBT_LEFT    = 3,
    BBT_ENTER   = 4,
    BBT_BACK    = 5
};


class Button : public OneButton
{
private:
    
    BaseButtonType _btnType = BBT_NONE;

public:
    
    Button(const int pin, const boolean activeLow = true, const bool pullupActive = true);
    
    ~Button();

    void setBtnType(BaseButtonType _type){_btnType = _type;}

    BaseButtonType type(){return _btnType;}

};




#endif //__BUTTON_H__