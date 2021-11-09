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


#include "ByteProtocol.h"
#include <Arduino.h>

void ByteProtocol::popBuffer(const uint8_t **buffer, uint32_t size)
{
    if (_decodeBytePointer + size > _decodeByteSize) {
        _decodeError = true;
        *buffer = nullptr;
        return;
    }
    *buffer = (uint8_t *)(_decodeBuffer + _decodeBytePointer);
    _decodeBytePointer+=size;
}

void ByteProtocol::setBit(uint8_t* b,uint8_t w)
{
    (*b)|=(1<<w);
}

void ByteProtocol::clrBit(uint8_t* b,uint8_t w)
{
    (*b)&=~(1<<w);
}

void ByteProtocol::setBit(uint8_t* b,uint8_t w,bool v)
{
    if (v){setBit(b,w);}
    else {clrBit(b,w);}
}

bool ByteProtocol::getBit(uint8_t b,uint8_t w)
{
    return (b >> w) & 0x01;
}

ByteProtocol::ByteProtocol()
{
    
}

ByteProtocol::~ByteProtocol()
{
    
}

ByteProtocol& ByteProtocol::enBegin()
{
    _encodeVector.clear();
    _encodeVector.reserve(32);
    return *this;
}

ByteProtocol& ByteProtocol::enByteBegin()
{
    _encodeByte = 0;
    _encodeBitPointer = 7;
    return *this;
}

ByteProtocol& ByteProtocol::pushBit(bool v)
{
    setBit(&_encodeByte, _encodeBitPointer--,v);
    return *this;
}

ByteProtocol& ByteProtocol::pushByte()
{
    pushUint8(_encodeByte);
    return *this;
}

ByteProtocol& ByteProtocol::pushUint8(uint8_t v)
{
    _encodeVector.push_back(v);
    return *this;
}

ByteProtocol& ByteProtocol::pushUint16(uint16_t v)
{
    _encodeVector.push_back(v>>8 & 0xFF);
    _encodeVector.push_back(v & 0xFF);
    return *this;
}

ByteProtocol& ByteProtocol::pushUint24(uint32_t v)
{
    _encodeVector.push_back(v>>16 & 0xFF);
    _encodeVector.push_back(v>>8 & 0xFF);
    _encodeVector.push_back(v & 0xFF);
    return *this;
}

ByteProtocol& ByteProtocol::pushUint32(uint32_t v)
{
    _encodeVector.push_back(v>>24 & 0xFF);
    _encodeVector.push_back(v>>16 & 0xFF);
    _encodeVector.push_back(v>>8 & 0xFF);
    _encodeVector.push_back(v & 0xFF);
    return *this;
}

ByteProtocol& ByteProtocol::pushInt8(int8_t v)
{
    return pushUint8(v);
}

ByteProtocol& ByteProtocol::pushInt16(int16_t v)
{
    return pushUint16(v);
}

ByteProtocol& ByteProtocol::pushInt32(int32_t v)
{
    return pushUint32(v);
}

ByteProtocol& ByteProtocol::pushString8(const std::string& v)
{
    uint8_t length = v.length();
    pushUint8(length);
    for (int16_t i = 0; i<length; i++) {
        _encodeVector.push_back(v.c_str()[i]);
    }
    return *this;
}

ByteProtocol& ByteProtocol::pushString16(const std::string& v)
{
    uint16_t length = v.length();
    pushUint16(length);
    const char* str = v.c_str();
    for (int32_t i = 0; i<length; i++) {
        _encodeVector.push_back(str[i]);
    }
    return *this;
}

ByteProtocol& ByteProtocol::pushBuffer8(uint8_t *buffer, uint8_t size)
{
    pushUint8(size);
    for (int16_t i = 0; i<size; i++) {
        _encodeVector.push_back(buffer[i]);
    }
    return *this;
}

ByteProtocol& ByteProtocol::pushBuffer16(uint8_t *buffer, uint16_t size)
{
    pushUint16(size);
    for (int32_t i = 0; i<size; i++) {
        _encodeVector.push_back(buffer[i]);
    }
    return *this;
}

ByteProtocol& ByteProtocol::pushBuffer32(uint8_t *buffer, uint32_t size)
{
    pushUint32(size);
    for (int32_t i = 0; i<size; i++) {
        _encodeVector.push_back(buffer[i]);
    }
    return *this;
}

ByteProtocol& ByteProtocol::pushProtocol(ByteProtocol& proto)
{
    uint32_t size = proto.getEncodeBufferSize();
    for (uint32_t i = 0; i < size; i++)
    {
        pushUint8(proto.getUint8(i));
    }
    return *this;
}

void ByteProtocol::enEnd(uint8_t** buffer,uint32_t* size)
{
    *size = (uint32_t)_encodeVector.size();
    *buffer = (uint8_t *)malloc(*size);
    for (uint32_t i = 0; i<*size; i++) {
        (*buffer)[i] = _encodeVector.at(i);
    }
}

uint8_t ByteProtocol::getUint8(uint32_t index)
{
    return _encodeVector.at(index);
}

uint32_t ByteProtocol::getEncodeBufferSize()
{
    return (uint32_t)_encodeVector.size();
}

void ByteProtocol::deBegin(const uint8_t* buffer,uint32_t size)
{
    _decodeBuffer = buffer;
    _decodeByteSize = size;
    _decodeBytePointer = 0;
    _decodeError = false;
}

uint8_t ByteProtocol::popUint8(uint8_t dv)
{
    if (_decodeBytePointer + 1 > _decodeByteSize)
    {
        return dv;
    }
    return _decodeBuffer[_decodeBytePointer++];
}

uint16_t ByteProtocol::popUint16(uint16_t dv)
{
    if (_decodeBytePointer + 2 > _decodeByteSize)
    {
        return dv;
    }
    uint16_t res
    =(uint16_t(_decodeBuffer[_decodeBytePointer]) << 8)
    +uint16_t(_decodeBuffer[_decodeBytePointer+1]);
    _decodeBytePointer+=2;
    return res;
}

uint32_t ByteProtocol::popUint32(uint32_t dv)
{
    if (_decodeBytePointer + 4 > _decodeByteSize)
    {
        return dv;
    }
    uint32_t res
    =(uint32_t(_decodeBuffer[_decodeBytePointer])   << 24)
    +(uint32_t(_decodeBuffer[_decodeBytePointer+1]) << 16)
    +(uint32_t(_decodeBuffer[_decodeBytePointer+2]) << 8)
    +uint32_t(_decodeBuffer[_decodeBytePointer+3]);
    _decodeBytePointer+=4;
    return res;
}

int8_t ByteProtocol::popInt8(int8_t dv)
{
    if (_decodeBytePointer + 1 > _decodeByteSize)
    {
        return dv;
    }
    return popUint8();
}

int16_t ByteProtocol::popInt16(int16_t dv)
{
    if (_decodeBytePointer + 2 > _decodeByteSize)
    {
        return dv;
    }
    return popUint16();
}

int32_t ByteProtocol::popInt32(int32_t dv)
{
    if (_decodeBytePointer + 4 > _decodeByteSize)
    {
        return dv;
    }
    return popUint32();
}

std::string ByteProtocol::popString8(const std::string& dv)
{
    if (_decodeBytePointer + 1 > _decodeByteSize)
    {
        return dv;
    }
    uint8_t length = popUint8();
    if (_decodeBytePointer + length > _decodeByteSize) {
        _decodeError = true;
        return dv;
    }
    std::string res = "";
    res.append((char *)(_decodeBuffer+_decodeBytePointer), length);
    _decodeBytePointer+=length;
    return res;
}

std::string ByteProtocol::popString16(const std::string& dv)
{
    if (_decodeBytePointer + 2 > _decodeByteSize)
    {
        return dv;
    }
    uint16_t length = popUint16();
    if (_decodeBytePointer + length > _decodeByteSize) {
        _decodeError = true;
        return dv;
    }
    std::string res = "";
    res.append((char *)(_decodeBuffer+_decodeBytePointer), length);
    _decodeBytePointer+=length;
    return res;
}

void ByteProtocol::popBuffer8(const uint8_t **buffer, uint8_t *size, const uint8_t *db, uint8_t ds)
{
    if (_decodeBytePointer + 1 > _decodeByteSize)
    {
        *size = ds;
        *buffer = db;
        return;
    }
    *size = popUint8();
    popBuffer(buffer, *size);
}

void ByteProtocol::popBuffer16(const uint8_t **buffer, uint16_t *size, const uint8_t *db, uint8_t ds)
{
    if (_decodeBytePointer + 2 > _decodeByteSize)
    {
        *size = ds;
        *buffer = db;
        return;
    }
    *size = popUint16();
    popBuffer(buffer, *size);
}

void ByteProtocol::popBuffer32(const uint8_t **buffer, uint32_t *size, const uint8_t *db, uint8_t ds)
{
    if (_decodeBytePointer + 4 > _decodeByteSize)
    {
        *size = ds;
        *buffer = db;
        return;
    }
    *size = popUint32();
    popBuffer(buffer, *size);
}

void ByteProtocol::popByte()
{
    if (_decodeBytePointer + 1 > _decodeByteSize)
    {
        _decodeByte = 0;
        _decodeBitPointer = UINT8_MAX;
        return;
    }
    _decodeByte = popUint8();
    _decodeBitPointer = 7;
}

bool ByteProtocol::popBit(bool dv)
{
    if (_decodeBitPointer == UINT8_MAX)
    {
        return dv;
    }
    return getBit(_decodeByte, _decodeBitPointer--);
}

uint32_t ByteProtocol::remainingDeBufferSize()
{
    return _decodeByteSize - _decodeBytePointer;
}
