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
#include "../dtFileUtils.h"

NS_DT_BEGIN


class FileutilsESP32 : public FileUtils
{

    friend class FileUtils;

protected:

    FileutilsESP32();

    bool init() override;

public:

    void setup(int8_t fss) override;

    void end(int8_t fss) override;

    virtual std::string readStringFromSpiffsPath(const std::string& filepath) const;

    virtual uint8_t* readDataFromSpiffsPath(const std::string& filepath, size_t* size) const;

    virtual bool writeStringToSpiffsPath(const std::string& content,const std::string& filepath) const;

    virtual bool writeDataToSpiffsPath(const uint8_t* content,ssize_t size,const std::string& filepath) const;

    virtual bool removeFileFromSpiffs(const std::string &filepath) const;

    virtual bool renameFileFromSpiffs(const std::string &oldFilepath, const std::string &newFilepath) const;
    
    virtual ssize_t getFileSizeFromSpiffs(const std::string &filepath) const;
    
    virtual bool isFileExistFromSpiffs(const std::string& filepath) const;

    virtual bool createDirectoryFromSpiffs(const std::string& dirPath) const;

    virtual bool removeDirectoryFromSpiffs(const std::string& dirPath) const;

    virtual bool formatSpiffs() const;
    
    virtual std::string readStringFromSdPath(const std::string& filepath) const;

    virtual uint8_t* readDataFromSdPath(const std::string& filepath, size_t* size) const;

    virtual bool writeStringToSdPath(const std::string& content,const std::string& filepath) const;

    virtual bool writeDataToSdPath(const uint8_t*content,ssize_t size,const std::string& filepath) const;

    virtual bool removeFileFromSd(const std::string &filepath) const;

    virtual bool renameFileFromSd(const std::string &oldFilepath, const std::string &newFilepath) const;
    
    virtual ssize_t getFileSizeFromSd(const std::string &filepath) const;
    
    virtual bool isFileExistFromSd(const std::string& filepath) const;

    virtual bool createDirectoryFromSd(const std::string& dirPath) const;

    virtual bool removeDirectoryFromSd(const std::string& dirPath) const;

    virtual bool formatSd() const;

};



NS_DT_END
