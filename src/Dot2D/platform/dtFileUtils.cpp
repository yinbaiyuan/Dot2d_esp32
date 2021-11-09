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
#include "dtFileUtils.h"

NS_DT_BEGIN

FileUtils* FileUtils::s_sharedFileUtils = nullptr;

void FileUtils::destroyInstance()
{
    DT_SAFE_DELETE(s_sharedFileUtils);
}

bool FileUtils::init()
{
    return true;
}

void FileUtils::setup(int8_t fss)
{
    
}

void FileUtils::end(int8_t fss)
{

}

FileUtils::FileUtils()
{

}

FileUtils::~FileUtils()
{

}

std::string FileUtils::readStringFromPath(const std::string& filepath,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return readStringFromSpiffsPath(filepath);
        }
            break;
        case FS_SD:
        {
            return readStringFromSdPath(filepath);
        }
            break;
        default:
            break;
    }
    return "";
}

uint8_t* FileUtils::readDataFromPath(const std::string& filepath, uint32_t* size,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return readDataFromSpiffsPath(filepath,size);
        }
            break;
        case FS_SD:
        {
            return readDataFromSdPath(filepath,size);
        }
            break;
        default:
            break;
    }
    return 0;
}

bool FileUtils::writeStringToPath(const std::string& content,const std::string& filepath,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return writeStringToSpiffsPath(content,filepath);
        }
            break;
        case FS_SD:
        {
            return writeStringToSdPath(content,filepath);
        }
            break;
        default:
            break;
    }
    return false;
}

bool FileUtils::writeDataToPath(const uint8_t* content,int32_t size,const std::string& filepath,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return writeDataToSpiffsPath(content,size,filepath);
        }
            break;
        case FS_SD:
        {
            return writeDataToSdPath(content,size,filepath);
        }
            break;
        default:
            break;
    }
    return false;
}

bool FileUtils::removeFile(const std::string &filepath,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return removeFileFromSpiffs(filepath);
        }
            break;
        case FS_SD:
        {
            return removeFileFromSd(filepath);
        }
            break;
        default:
            break;
    }
    return false;
}

bool FileUtils::renameFile(const std::string &oldFilepath, const std::string &newFilepath,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return renameFileFromSpiffs(oldFilepath,newFilepath);
        }
            break;
        case FS_SD:
        {
            return renameFileFromSd(oldFilepath,newFilepath);
        }
            break;
        default:
            break;
    }
    return false;
}

int32_t FileUtils::getFileSize(const std::string &filepath,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return getFileSizeFromSpiffs(filepath);
        }
            break;
        case FS_SD:
        {
            return getFileSizeFromSd(filepath);
        }
            break;
        default:
            break;
    }
    return 0;
}

bool FileUtils::isFileExist(const std::string& filepath,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return isFileExistFromSpiffs(filepath);
        }
            break;
        case FS_SD:
        {
            return isFileExistFromSd(filepath);
        }
            break;
        default:
            break;
    }
    return false;
}

bool FileUtils::createDirectory(const std::string& dirPath,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return createDirectoryFromSpiffs(dirPath);
        }
            break;
        case FS_SD:
        {
            return createDirectoryFromSd(dirPath);
        }
            break;
        default:
            break;
    }
    return false;
}

bool FileUtils::removeDirectory(const std::string& dirPath,FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return removeDirectoryFromSpiffs(dirPath);
        }
            break;
        case FS_SD:
        {
            return removeDirectoryFromSd(dirPath);
        }
            break;
        default:
            break;
    }
    return false;
}

bool FileUtils::format(FileSystem fs) const
{
    switch (fs)
    {
        case FS_SPIFFS:
        {
            return formatSpiffs();
        }
            break;
        case FS_SD:
        {
            return formatSd();
        }
            break;
        default:
            break;
    }
    return false;
}


NS_DT_END
