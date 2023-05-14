#ifndef _FILE_HOLDER_H_
#define _FILE_HOLDER_H_


#include <string>
#include "delete_file.h"
#include "read_file.h"
#include "save_changes.h"


class IFileHolder
{
public:
    virtual bool Save(std::string filePath, std::string fileChanges) = 0;
    virtual bool Delete(std::string filePath) = 0;
    virtual std::string Read(std::string filePath) = 0;

    virtial ~IFileHolder() {}

protected:
    SaveChanges m_changesSavior;
    DeleteFile m_fileDeletor;
    ReadFile m_fileReader;
};

#endif  // _FILE_HOLDER_H_
