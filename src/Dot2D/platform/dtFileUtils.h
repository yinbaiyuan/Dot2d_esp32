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
#ifndef __DT_FILEUTILS_H__
#define __DT_FILEUTILS_H__

#include "../base/dtMacros.h"
#include <string>

NS_DT_BEGIN

/**
 * @addtogroup platform
 * @{
 */

enum FileSystem
{
    FS_SPIFFS = 0,
    FS_SD
};

enum FileState
{
    FS_UNINIT = 0,
    FS_READY = 1,
    FS_ERROR
};

enum FileSystemState
{
    FSS_UNINIT  = 0,
    FSS_SPIFFS  = 1 << 0,
    FSS_SD      = 1 << 1
};


class FileUtils
{
protected:

    static FileUtils* s_sharedFileUtils;

    FileState _spiffs_state = FS_UNINIT;

    FileState _sd_state = FS_UNINIT;

    FileSystemState _fileSystemState = FSS_UNINIT;

protected:

    virtual bool init();

public:

    static FileUtils* getInstance();

    static void destroyInstance();

    virtual void setup(int8_t fss);

    virtual void end(int8_t fss);

    FileUtils();

    virtual ~FileUtils();

    std::string readStringFromPath(const std::string& filepath,FileSystem fs = FS_SPIFFS) const;

    uint8_t* readDataFromPath(const std::string& filepath,uint32_t* size,FileSystem fs = FS_SPIFFS) const;

    bool writeStringToPath(const std::string& content,const std::string& filepath,FileSystem fs = FS_SPIFFS) const;

    bool writeDataToPath(const uint8_t* content,int32_t size,const std::string& filepath,FileSystem fs = FS_SPIFFS) const;

    bool removeFile(const std::string &filepath,FileSystem fs = FS_SPIFFS) const;

    bool renameFile(const std::string &oldFilepath, const std::string &newFilepath,FileSystem fs = FS_SPIFFS) const;
    
    int32_t getFileSize(const std::string &filepath,FileSystem fs = FS_SPIFFS) const;
    
    bool isFileExist(const std::string& filepath,FileSystem fs = FS_SPIFFS) const;

    bool createDirectory(const std::string& dirPath,FileSystem fs = FS_SPIFFS) const;

    bool removeDirectory(const std::string& dirPath,FileSystem fs = FS_SPIFFS) const;

    bool format(FileSystem fs = FS_SPIFFS) const;

    FileState getSpiffsState(){return _spiffs_state;}

    FileState getSdState(){return _sd_state;}


    virtual std::string readStringFromSpiffsPath(const std::string& filepath) const{return "";}

    virtual uint8_t* readDataFromSpiffsPath(const std::string& filepath, uint32_t* size) const{return 0;}

    virtual bool writeStringToSpiffsPath(const std::string& content,const std::string& filepath) const{return false;}

    virtual bool writeDataToSpiffsPath(const uint8_t* content,int32_t size,const std::string& filepath) const{return false;}

    virtual bool removeFileFromSpiffs(const std::string &filepath) const{return false;}

    virtual bool renameFileFromSpiffs(const std::string &oldFilepath, const std::string &newFilepath) const{return false;}
    
    virtual int32_t getFileSizeFromSpiffs(const std::string &filepath) const{return 0;}
    
    virtual bool isFileExistFromSpiffs(const std::string& filepath) const{return false;}

    virtual bool createDirectoryFromSpiffs(const std::string& dirPath) const{return false;}

    virtual bool removeDirectoryFromSpiffs(const std::string& dirPath) const{return false;}

    virtual bool formatSpiffs() const{return false;}

    virtual std::string readStringFromSdPath(const std::string& filepath) const{return "";}

    virtual uint8_t* readDataFromSdPath(const std::string& filepath, uint32_t* size) const{return 0;}

    virtual bool writeStringToSdPath(const std::string& content,const std::string& filepath) const{return false;}

    virtual bool writeDataToSdPath(const uint8_t* content,int32_t size,const std::string& filepath) const{return false;}

    virtual bool removeFileFromSd(const std::string &filepath) const{return false;}

    virtual bool renameFileFromSd(const std::string &oldFilepath, const std::string &newFilepath) const{return false;}
    
    virtual int32_t getFileSizeFromSd(const std::string &filepath) const{return 0;}
    
    virtual bool isFileExistFromSd(const std::string& filepath) const{return false;}

    virtual bool createDirectoryFromSd(const std::string& dirPath) const{return false;}

    virtual bool removeDirectoryFromSd(const std::string& dirPath) const{return false;}

    virtual bool formatSd() const{return false;}

};


// end of support group
/** @} */

NS_DT_END


#endif //__DT_FILEUTILS_H__
