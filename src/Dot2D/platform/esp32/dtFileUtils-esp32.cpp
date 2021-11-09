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
#include "dtFileUtils-esp32.h"
#include <SPIFFS.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"

NS_DT_BEGIN

FileUtils* FileUtils::getInstance()
{
    if (s_sharedFileUtils == nullptr)
    {
        s_sharedFileUtils = new FileutilsESP32();
        if(!s_sharedFileUtils->init())
        {
          delete s_sharedFileUtils;
          s_sharedFileUtils = nullptr;
        }
    }
    return s_sharedFileUtils;
}

FileutilsESP32::FileutilsESP32()
{

}

bool FileutilsESP32::init()
{
    FileUtils::init();
    return true;
}

void FileutilsESP32::setup(int8_t fss)
{
    if(!((_fileSystemState >> 0) & 0x01) && ((fss >> 0) & 0x01))
    {
        /*SPIFFS INIT*/
        _spiffs_state = FS_ERROR;
        for (int8_t i = 0; i < 5; i++)
        {
            if(SPIFFS.begin(true))
            {
                _spiffs_state = FS_READY;
                _fileSystemState = (FileSystemState)(_fileSystemState | FSS_SPIFFS);
                break;
            }
        }
    }

    if(!((_fileSystemState >> 1) & 0x01) && ((fss >> 1) & 0x01))
    {
        /*SD INIT*/
        _sd_state = FS_ERROR;
        if(SD.begin())
        {
            uint8_t cardType = SD.cardType();
            if(cardType != CARD_NONE)
            {
                _sd_state = FS_READY;
                _fileSystemState = (FileSystemState)(_fileSystemState | FSS_SD);
            }
        }
    }
}



void FileutilsESP32::end(int8_t fss)
{
    if(((_fileSystemState >> 0) & 0x01) && ((fss >> 0) & 0x01))
    {
        _spiffs_state = FS_UNINIT;
        _fileSystemState = (FileSystemState)(_fileSystemState & (~FSS_SPIFFS) );
        SPIFFS.end();
        
    }

    if(((_fileSystemState >> 1) & 0x01) && ((fss >> 1) & 0x01))
    {
        _sd_state = FS_UNINIT;
         _fileSystemState = (FileSystemState)(_fileSystemState & (~FSS_SD));
        SD.end();
    }
}


std::string FileutilsESP32::readStringFromSpiffsPath(const std::string& filepath) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return "";
    }
    File file = SPIFFS.open(filepath.c_str(), FILE_READ);
    if(!file){return "";}
    String data = file.readString();
    file.close();
    return std::string(data.c_str(),data.length());
}

uint8_t* FileutilsESP32::readDataFromSpiffsPath(const std::string& filepath, size_t* size) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        *size = 0;
        return nullptr;
    }
    File file = SPIFFS.open(filepath.c_str(), FILE_READ);
    if(!file){*size = 0; return nullptr;}

    size_t len = file.size();
    uint8_t* buffer = (uint8_t* )malloc(len);
    size_t readLen = file.readBytes((char *)buffer,len);
    file.close();
    if (readLen < len)
    {
        delete buffer;
        *size = 0;
        return nullptr;
    }
    *size = len;
    return buffer;
}

bool FileutilsESP32::writeStringToSpiffsPath(const std::string& content,const std::string& filepath) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return false;
    }

    File file = SPIFFS.open(filepath.c_str(), FILE_WRITE);
    if(!file){return false;}
    file.write((const uint8_t*)content.c_str(),content.size());
    file.close();
    return true;
}

bool FileutilsESP32::writeDataToSpiffsPath(const uint8_t* content,ssize_t size,const std::string& filepath) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return false;
    }
    File file = SPIFFS.open(filepath.c_str(), FILE_WRITE);
    if(!file){return false;}
    file.write(content,size);
    file.close();
    return true;
}

bool FileutilsESP32::removeFileFromSpiffs(const std::string &filepath) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return false;
    }
    
    return SPIFFS.remove(filepath.c_str());;
}

bool FileutilsESP32::renameFileFromSpiffs(const std::string &oldFilepath, const std::string &newFilepath) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return false;
    }
    
    return SPIFFS.rename(oldFilepath.c_str(), newFilepath.c_str());
}

ssize_t FileutilsESP32::getFileSizeFromSpiffs(const std::string &filepath) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return 0;
    }

    File file = SPIFFS.open(filepath.c_str());
    if(!file){return 0;}
    ssize_t len = file.size();
    file.close();
    return len;
}

bool FileutilsESP32::isFileExistFromSpiffs(const std::string& filepath) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return false;
    }

    return SPIFFS.exists(filepath.c_str());
}

bool FileutilsESP32::createDirectoryFromSpiffs(const std::string& dirPath) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return false;
    }
    return false;
}

bool FileutilsESP32::removeDirectoryFromSpiffs(const std::string& dirPath) const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return false;
    }
    return false;
}

bool FileutilsESP32::formatSpiffs() const
{
    if(!(_fileSystemState & FSS_SPIFFS))
    {
        return false;
    }
    return SPIFFS.format();
}

std::string FileutilsESP32::readStringFromSdPath(const std::string& filepath) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return "";
    }
    File file = SD.open(filepath.c_str(),FILE_READ);
    if(!file){return "";}
    String data = file.readString();
    file.close();
    return std::string(data.c_str(),data.length());;
}

uint8_t* FileutilsESP32::readDataFromSdPath(const std::string& filepath, size_t* size) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        *size = 0;
        return nullptr;
    }

    File file = SD.open(filepath.c_str(), FILE_READ);
    if(!file){*size = 0; return nullptr;}

    size_t len = file.size();
    uint8_t* buffer = (uint8_t* )malloc(len);
    size_t readLen = file.readBytes((char *)buffer,len);
    file.close();
    if (readLen < len)
    {
        delete buffer;
        *size = 0;
        return nullptr;
    }
    *size = len;
    return buffer;
}

bool FileutilsESP32::writeStringToSdPath(const std::string& content,const std::string& filepath) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return false;
    }
    File file = SD.open(filepath.c_str(), FILE_WRITE);
    if(!file){return false;}
    file.write((const uint8_t*)content.c_str(),content.size());
    file.close();
    return true;
}

bool FileutilsESP32::writeDataToSdPath(const uint8_t* content,ssize_t size,const std::string& filepath) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return false;
    }
    File file = SD.open(filepath.c_str(), FILE_WRITE);
    if(!file){return false;}
    file.write(content,size);
    file.close();
    return true;
}

bool FileutilsESP32::removeFileFromSd(const std::string &filepath) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return false;
    }

    return SD.remove(filepath.c_str());;
}

bool FileutilsESP32::renameFileFromSd(const std::string &oldFilepath, const std::string &newFilepath) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return false;
    }
    return SD.rename(oldFilepath.c_str(), newFilepath.c_str());
}

ssize_t FileutilsESP32::getFileSizeFromSd(const std::string &filepath) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return 0;
    }
    File file = SD.open(filepath.c_str());
    if(!file){return 0;}
    ssize_t len = file.size();
    file.close();
    return len;
}

bool FileutilsESP32::isFileExistFromSd(const std::string& filepath) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return false;
    }
    return SD.exists(filepath.c_str());
}

bool FileutilsESP32::createDirectoryFromSd(const std::string& dirPath) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return false;
    }
    return SD.mkdir(dirPath.c_str());
}

bool FileutilsESP32::removeDirectoryFromSd(const std::string& dirPath) const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return false;
    }
    return SD.rmdir(dirPath.c_str());
}

bool FileutilsESP32::formatSd() const
{
    if(!(_fileSystemState & FSS_SD))
    {
        return false;
    }
    return false;
}

NS_DT_END