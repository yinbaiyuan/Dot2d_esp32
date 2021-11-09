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
#include "DgfParser.h"

void DgfEncoder::freeEncoder()
{
    this->freeFrameInfoChian(_frameChain);
    _frameChain = nullptr;
    this->freeDgfRGBChian(_colorChain);
    _colorChain = nullptr;
}

bool DgfEncoder::setFrameToBuffer(DgfFrameInfoChain* chain,uint8_t* buffer,uint32_t* dataPointer)
{
    buffer[(*dataPointer)++] = DGF_GRAPH_FRAME_HEAD;
    buffer[(*dataPointer)++] = chain->info.duration >> 8 & 0xFF;
    buffer[(*dataPointer)++] = chain->info.duration & 0xFF;
    buffer[(*dataPointer)++] = chain->info.x;
    buffer[(*dataPointer)++] = chain->info.y;
    buffer[(*dataPointer)++] = chain->info.width;
    buffer[(*dataPointer)++] = chain->info.height;
    
    uint8_t frameTransparenceInfo = (chain->info.transparenceType & 0x03)<<6;
    frameTransparenceInfo += chain->info.transparenceColorIndex & 0b00111111;
    buffer[(*dataPointer)++] = frameTransparenceInfo;
    
    uint8_t disposalInfo = (chain->info.disposalMode & 0x0F)<<4;
    disposalInfo += chain->info.disposalParameter & 0b00001111;
    buffer[(*dataPointer)++] = disposalInfo;
    
    if (chain->info.disposalMode == 2) {
        buffer[(*dataPointer)++] = chain->info.translateX;
        buffer[(*dataPointer)++] = chain->info.translateY;
    }

    uint16_t framePixelCount = chain->info.width * chain->info.height;
    uint8_t colorIndexType = this->getColorIndexType();
    switch (colorIndexType)
    {
    case 0:
    {
        uint8_t tempByte = 0;
        for (int32_t i = 0; i < framePixelCount; i++)
        {
            if (chain->info.data[i] >= _colorCount){return false;}
            this->set_bit(&tempByte,7-i%8,chain->info.data[i]);
            if (i%8==7 || i == framePixelCount-1)
            {
                buffer[(*dataPointer)++] = tempByte;
                tempByte = 0;
            }
        }
    }
        break;
    case 1:
    {
        uint8_t tempByte = 0;
        for (int32_t i = 0; i < framePixelCount; i++)
        {
            if (chain->info.data[i] >= _colorCount){return false;}
            uint8_t w = (3 - i%4) * 2;
            this->set_bit(&tempByte,w+1,(chain->info.data[i]>>1) & 0x01);
            this->set_bit(&tempByte,w,(chain->info.data[i]>>0) & 0x01);
            if (i%4==3 || i == framePixelCount-1)
            {
                buffer[(*dataPointer)++] = tempByte;
                tempByte = 0;
            }
        }
    }
        break;
    case 2:
    {
        uint8_t tempByte = 0;
        for (int32_t i = 0; i < framePixelCount; i++)
        {
            if (chain->info.data[i] >= _colorCount){return false;}
            uint8_t w = (1 - i%2) * 4;
            this->set_bit(&tempByte,w+3,(chain->info.data[i]>>3) & 0x01);
            this->set_bit(&tempByte,w+2,(chain->info.data[i]>>2) & 0x01);
            this->set_bit(&tempByte,w+1,(chain->info.data[i]>>1) & 0x01);
            this->set_bit(&tempByte,w,(chain->info.data[i]>>0) & 0x01);
            if (i%2==1 || i == framePixelCount-1)
            {
                buffer[(*dataPointer)++] = tempByte;
                tempByte = 0;
            }
        }
    }
        break;
    case 3:
    {
        uint8_t tempByte = 0;
        for (int32_t i = 0; i < framePixelCount; i++)
        {
            if (chain->info.data[i] >= _colorCount){return false;}
            switch (i%4)
            {
            case 0:
            {
                tempByte += chain->info.data[i];
                tempByte = tempByte<<2;
                if (i == framePixelCount-1)
                {
                    buffer[(*dataPointer)++] = tempByte;
                    tempByte = 0;
                }
            }
                break;
            case 1:
            {
                tempByte += (chain->info.data[i] >> 4 & 0b00000011);
                buffer[(*dataPointer)++] = tempByte;
                tempByte = (chain->info.data[i] & 0b00001111);
                tempByte = tempByte<<4;
                if (i == framePixelCount-1)
                {
                    buffer[(*dataPointer)++] = tempByte;
                    tempByte = 0;
                }
            }
                break;
            case 2:
            {
                tempByte += (chain->info.data[i] >> 2 & 0b00001111);
                buffer[(*dataPointer)++] = tempByte;
                tempByte = (chain->info.data[i] & 0b00000011);
                tempByte = tempByte<<6;
                if (i == framePixelCount-1)
                {
                    buffer[(*dataPointer)++] = tempByte;
                    tempByte = 0;
                }
            }
                break;
            case 3:
            {
                tempByte += (chain->info.data[i] & 0b00111111);
                buffer[(*dataPointer)++] = tempByte;
                tempByte = 0;
            }
                break;
            
            default:
                break;
            }
        }
    }
        break;
    default:
        break;
    }
    return true;
}

void DgfEncoder::freeFrameInfoChian(DgfFrameInfoChain* chain)
{
    if (!chain){return;}
    if ((chain->info).data) {
        free((chain->info).data);
    }
    this->freeFrameInfoChian(chain->next);
    free(chain);
}

void DgfEncoder::freeDgfRGBChian(DgfRGBChian* chain)
{
    if (!chain){return;}
    this->freeDgfRGBChian(chain->next);
    free(chain);
}

DgfFrameInfoChain* DgfEncoder::getLastFrameInfoChian()
{
    DgfFrameInfoChain* last = _frameChain;
    if (!last)
    {
        return nullptr;
    }
    while (last->next)
    {
        last = last->next;
    }
    return last;
}

DgfRGBChian* DgfEncoder::getLastDgfRGBChian()
{
    DgfRGBChian* last = _colorChain;
    if (!last)
    {
        return nullptr;
    }
    while (last->next)
    {
        last = last->next;
    }
    return last;
}

uint32_t DgfEncoder::getFrameChainLenth(DgfFrameInfoChain* chain)
{
    if (!chain){return 0;}
    uint32_t res = 9;
    if (chain->info.disposalMode == 2) {
        res += 2;
    }
    uint16_t framePixelCount = chain->info.width * chain->info.height;
    uint8_t colorIndexType = this->getColorIndexType();
    switch (colorIndexType)
    {
    case 0:
    {
        res += (framePixelCount + 7) / 8;
    }
        break;
    case 1:
    {
        res += (framePixelCount + 3) / 4;
    }
        break;
    case 2:
    {
        res += (framePixelCount + 1) / 2;
    }
        break;
    case 3:
    {
        res += framePixelCount / 4 * 3 + framePixelCount % 4;
    }
        break;
    default:
        break;
    }
    res += getFrameChainLenth(chain->next);
    return res;
}

uint32_t DgfEncoder::computeBufferLength()
{
    uint32_t res = 4;                       //文件头
    res += 3;                               //颜色索引表头
    res += _colorCount * 3;                 //颜色索引
    res += getFrameChainLenth(_frameChain); //帧数据
    res += 1;                               //文件尾
    return res;
}

int8_t DgfEncoder::getColorIndexType()
{
    if (_colorCount <= 2)
    {
        return 0;
    }
    if (_colorCount <= 4)
    {
        return 1;
    }
    if (_colorCount <= 16)
    {
        return 2;
    }
    if (_colorCount <= 64)
    {
        return 3;
    }
    return -1;
}

DgfEncoder::DgfEncoder()
{
    
}

DgfEncoder::~DgfEncoder()
{
    freeEncoder();
}

void DgfEncoder::setSize(uint8_t width,uint8_t height)
{
    _width = width;
    _height = height;
}

bool DgfEncoder::addColor(uint8_t r,uint8_t g,uint8_t b)
{
    if (_colorCount>=64)
    {
        return false;
    }
    _colorCount++;
    DgfRGBChian *newOne = new DgfRGBChian();
    newOne->color.r = r;
    newOne->color.g = g;
    newOne->color.b = b;
    newOne->next = nullptr;
    DgfRGBChian *last = this->getLastDgfRGBChian();
    if (last)
    {
        last->next = newOne;
    }else
    {
        _colorChain = newOne;
    }
    return true;
}

bool DgfEncoder::addFrame(uint16_t duration,                //持续时间
                          uint8_t x,                        //需要修改的图片数据区域的左上点X坐标
                          uint8_t y,                        //需要修改的图片数据区域的左上点Y坐标
                          uint8_t width,                    //需要修改的图片数据区域的宽度
                          uint8_t height,                   //需要修改的图片数据区域的高度
                          uint8_t transparenceType,         //当前帧透明像素处理方式
                          uint8_t transparenceColorIndex,   //当前透明像素索引值
                          uint8_t disposalMode,             //上一帧数据渲染处置方式
                          uint8_t disposalParameter,        //处置方式参数
                          int8_t translateX,                //上一帧X轴平移值
                          int8_t translateY,                //上一帧Y轴平移值
                          uint8_t* bitmap)
{
    if (_width < width + x || _height < height + y)
    {
        return false;
    }
    DgfFrameInfoChain * newOne = new DgfFrameInfoChain();
    newOne->info.x = x;
    newOne->info.y = y;
    newOne->info.width = width;
    newOne->info.height = height;
    newOne->info.duration = duration;
    newOne->info.transparenceType = transparenceType;
    newOne->info.transparenceColorIndex = transparenceColorIndex;
    newOne->info.disposalMode = disposalMode;
    newOne->info.disposalParameter = disposalParameter;
    newOne->info.translateX = translateX;
    newOne->info.translateY = translateY;
    uint16_t pixelCount = width * height;
    newOne->info.data = (uint8_t*)malloc(pixelCount);
    memcpy(newOne->info.data, bitmap, pixelCount);
    DgfFrameInfoChain* last = this->getLastFrameInfoChian();
    if (last)
    {
        last->next = newOne;
    }else
    {
        _frameChain = newOne;
    }
    return true;
}

uint8_t* DgfEncoder::encode(uint32_t* size)
{
    bool dataCheck = true;
    if (_colorCount<1){dataCheck = false;}
    else if (_colorCount>64){dataCheck = false;}
    else if (!_frameChain){dataCheck = false;}
    else if (_width<1 || _height < 1){dataCheck = false;}

    if (!dataCheck)
    {
        *size = 0;
        return nullptr;
    }
    
    uint32_t s = this->computeBufferLength();
    uint8_t* buffer = (uint8_t*)malloc(s);
    if (!buffer)
    {
        *size = 0;
        return nullptr;
    }
    uint32_t dataPointer = 0;
    buffer[dataPointer++] = 'D';
    buffer[dataPointer++] = 'G';
    buffer[dataPointer++] = 'F';
    buffer[dataPointer++] = EncoderVersion;
    buffer[dataPointer++] = _width;
    buffer[dataPointer++] = _height;
    uint8_t colorInfo = this->getColorIndexType() <<6;
    colorInfo += (_colorCount - 1);
    buffer[dataPointer++] = colorInfo;
    
    DgfRGBChian *rgbChain = _colorChain;
    while (rgbChain)
    {
        buffer[dataPointer++] = rgbChain->color.r;
        buffer[dataPointer++] = rgbChain->color.g;
        buffer[dataPointer++] = rgbChain->color.b;
        rgbChain = rgbChain->next;
    }
    DgfFrameInfoChain *frameInfoChain = _frameChain;
    while (frameInfoChain)
    {
        if (!this->setFrameToBuffer(frameInfoChain,buffer,&dataPointer))
        {
            free(buffer);
            *size = 0;
            return nullptr;
        }
        frameInfoChain = frameInfoChain->next;
    }
    buffer[dataPointer++] = 0b10010110;
    *size = s;
    return buffer;
}


///////////////////////////////////////////////////////////////////////////////////////


void DgfDecoder::freeDecoder()
{
    _frameChainPointer = nullptr;
    free(_colorTable);
    _colorTable = nullptr;
    freeDgfRGBChian(_colorChain);
    _colorChain = nullptr;
    freeFrameInfoChian(_frameChain);
    _frameChain = nullptr;
    free(_screenBuffer);
    _screenBuffer = nullptr;
}

bool DgfDecoder::getFrameFromBuffer(DgfFrameInfoChain* chain,const uint8_t* bitmap,uint32_t* dataPointer,uint32_t totalSize)
{
    //TODO 根据totalSize进行数据长度合法性校验
    uint8_t frameHead = bitmap[(*dataPointer)++];
    if (frameHead != DGF_GRAPH_FRAME_HEAD) {
        return false;
    }
    _framesCount++;
    DgfFrameInfo* info = &(chain->info);
    uint16_t duration = bitmap[(*dataPointer)++];
    duration = (duration << 8) + bitmap[(*dataPointer)++];
    info->duration = duration;
    info->x = bitmap[(*dataPointer)++];
    info->y = bitmap[(*dataPointer)++];
    info->width = bitmap[(*dataPointer)++];
    info->height = bitmap[(*dataPointer)++];
    uint8_t transparenceInfo = bitmap[(*dataPointer)++];
    info->transparenceType = (transparenceInfo >> 6) & 0x03;
    info->transparenceColorIndex = transparenceInfo & 0b00111111;
    uint8_t disposalType = bitmap[(*dataPointer)++];
    info->disposalMode = (disposalType >> 4) & 0x0F;
    info->disposalParameter = disposalType & 0x0F;
    if (info->disposalMode == 2) {
        info->translateX = bitmap[(*dataPointer)++];
        info->translateY = bitmap[(*dataPointer)++];
    }
    uint16_t framePixelCount = info->width * info->height;
    info->data = (uint8_t *)malloc(framePixelCount);
    switch (_colorType)
    {
    case 0:
    {
        uint8_t tempByte = 0;
        for (int32_t i = 0; i < framePixelCount; i++)
        {
            if (i%8==0)
            {
                tempByte = bitmap[(*dataPointer)++];
            }
            info->data[i] = tempByte >> (7 - (i%8)) & 0b00000001;
        }
    }
        break;
    case 1:
    {
        uint8_t tempByte = 0;
        for (int32_t i = 0; i < framePixelCount; i++)
        {
            if (i%4==0)
            {
                tempByte = bitmap[(*dataPointer)++];
            }
            info->data[i] = tempByte >> (6 - (i%4)*2) & 0b00000011;
        }
    }
        break;
    case 2:
    {
        uint8_t tempByte = 0;
        for (int32_t i = 0; i < framePixelCount; i++)
        {
            if (i%2==0)
            {
                tempByte = bitmap[(*dataPointer)++];
            }
            info->data[i] = tempByte >> (4 - (i%2)*4) & 0b00001111;
        }
    }
        break;
    case 3:
    {
        uint8_t lastByte = 0;
        for (int32_t i = 0; i < framePixelCount; i++)
        {
            switch (i%4)
            {
            case 0:
            {
                uint8_t tempByte = bitmap[(*dataPointer)++];
                info->data[i] = tempByte >> 2 & 0b00111111;
                lastByte = (tempByte & 0b00000011) << 4;
            }
                break;
            case 1:
            {
                uint8_t tempByte = bitmap[(*dataPointer)++];
                info->data[i] = lastByte + (tempByte >> 4 & 0b00001111);
                lastByte = (tempByte & 0b00001111) << 2;
            }
                break;
            case 2:
            {
                uint8_t tempByte = bitmap[(*dataPointer)++];
                info->data[i] = lastByte + (tempByte >> 6 & 0b00000011);
                lastByte = tempByte & 0b00111111;
            }
                break;
            case 3:
            {
                info->data[i] = lastByte;
            }
                break;
            default:
                break;
            }
        }
    }
        break;
    default:
        break;
    }
    uint8_t nextFrameHead = bitmap[(*dataPointer)];
    if (nextFrameHead == DGF_GRAPH_FRAME_HEAD) {
        chain->next = new DgfFrameInfoChain();
        return getFrameFromBuffer(chain->next,bitmap,dataPointer,totalSize);
    }
    return true;
}

void DgfDecoder::renderFrame()
{
    if (!_frameChainPointer) {
        return;
    }
    DgfFrameInfo& frameInfo = _frameChainPointer->info;
    switch (frameInfo.disposalMode) {
        case 0:
        {
            uint16_t dotCount = _width*_height;
            if (frameInfo.transparenceType == 0) {
                memset(_screenBuffer, frameInfo.transparenceColorIndex, dotCount);
            }else
            {
                memset(_screenBuffer, 0xFF, dotCount);
            }
        }
            break;
        case 1:
        {
            //do nothing
        }
            break;
        case 2:
        {
            uint16_t dotCount = _width*_height;
            uint8_t* tempBuffer = (uint8_t *)malloc(dotCount);
            memcpy(tempBuffer, _screenBuffer, dotCount);
            if (frameInfo.transparenceType == 0) {
                memset(_screenBuffer, frameInfo.transparenceColorIndex, dotCount);
            }else
            {
                memset(_screenBuffer, 0xFF, dotCount);
            }
            memset(_screenBuffer, frameInfo.transparenceColorIndex, dotCount);
            int16_t dstX = frameInfo.translateX>=0?frameInfo.translateX:0;
            int16_t dstY = (-frameInfo.translateY<=0)?0:frameInfo.translateY;
            for (int16_t y = dstY; y < _height; y++) {
                for (int16_t x = dstX; x < _width; x++)
                {
                    int16_t oX = x - frameInfo.translateX;
                    int16_t oY = y + frameInfo.translateY;
                    uint16_t index = y * _width + x;
                    _screenBuffer[index] = tempBuffer[oY * _width + oX];
                }
            }
            free(tempBuffer);
        }
            break;
            
        default:
            break;
    }
    for (int16_t fx = 0; fx < frameInfo.width; fx++) {
        for (int16_t fy = 0; fy < frameInfo.height; fy++) {
            uint16_t x = fx + frameInfo.x;
            uint16_t y = fy + frameInfo.y;
            uint8_t colorIndex = frameInfo.data[fy*frameInfo.width+fx];
            if (frameInfo.transparenceColorIndex == colorIndex) {
                if (frameInfo.transparenceType == 2)
                {
                    
                }
                else if (frameInfo.transparenceType == 1)
                {
                    _screenBuffer[y * _width + x] = 0xFF;
                }
            }else
            {
                _screenBuffer[y * _width + x] = colorIndex;
            }
        }
    }
    if(frameInfo.duration == 0)
    {
        renderNextFrame(true);
    }
}


DgfDecoder::DgfDecoder()
{

}

DgfDecoder::~DgfDecoder()
{
    freeDecoder();
}

bool DgfDecoder::setNextFrame(bool loop)
{
    _frameChainPointer = _frameChainPointer->next;
    if (!_frameChainPointer) {
        if (loop) {
            _frameChainPointer = _frameChain;
            return true;
        }else
        {
            return false;
        }
    }
    return true;
}

bool DgfDecoder::renderNextFrame(bool loop)
{
    bool res = setNextFrame(loop);
    this->renderFrame();
    return res;
}

bool DgfDecoder::decode(const uint8_t* bitmap,uint32_t size)
{
    freeDecoder();
    if (size < 8){return false;}
    uint32_t dataPointer = 0;
    if (bitmap[dataPointer++] != 'D' || 
        bitmap[dataPointer++] != 'G' || 
        bitmap[dataPointer++] != 'F'){return false;}
    _version = bitmap[dataPointer++];
    if (_version != 0b00010001){return false;}
    _width = bitmap[dataPointer++];
    _height = bitmap[dataPointer++];
    uint8_t colorInfo = bitmap[dataPointer++];
    _colorType = colorInfo>>6 & 0b00000011;
    _colorCount = (colorInfo & 0b00111111) + 1;
    if((_colorTable = (DgfRGB *)malloc(sizeof(DgfRGB) * _colorCount)) == nullptr)
    {
        freeDecoder();
        return false;
    }
    for (uint8_t i = 0; i < _colorCount; i++)
    {
        _colorTable[i].r = bitmap[dataPointer++];
        _colorTable[i].g = bitmap[dataPointer++];
        _colorTable[i].b = bitmap[dataPointer++];
    }
    _frameChain = new DgfFrameInfoChain();
    if (!this->getFrameFromBuffer(_frameChain,bitmap,&dataPointer,size)) {
        freeDecoder();
        return false;
    }
    if((_screenBuffer = (uint8_t *)malloc(_width*_height)) == nullptr)
    {
        freeDecoder();
        return false;
    }
    _frameChainPointer = _frameChain;
    this->renderFrame();
    return true;
}

uint8_t DgfDecoder::getColorIndex(uint8_t x,uint8_t y) const
{
    if (x >= _width ||
        y >= _height)
    {
        return 0;
    }
    return _screenBuffer[y * _width + x];
}

uint32_t DgfDecoder::getColor(uint8_t x,uint8_t y) const
{
    return getColorByIndex(getColorIndex(x,y));
}

void DgfDecoder::setColor(uint8_t index,uint8_t r,uint8_t g,uint8_t b)
{
    if (index>=_colorCount) {
        return;
    }
    _colorTable[index].r = r;
    _colorTable[index].g = g;
    _colorTable[index].b = b;
}

uint32_t DgfDecoder::getColorByIndex(uint8_t index) const
{
    if (index == 0xFF) {
        return 0xFF000000;
    }
    DgfRGB& color = _colorTable[index];
    return color.getColor();
}

uint16_t DgfDecoder::getDuration() const
{
    if (!_frameChainPointer) {
        return 0;
    }
    return _frameChainPointer->info.duration;
}
