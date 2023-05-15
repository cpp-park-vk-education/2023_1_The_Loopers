#ifndef _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_


#include <string>
#include "delete_file.h"
#include "read_file.h"
#include "save_changes.h"


namespace inklink_file_holder
{
class IFileHolder
{
public:
    virtual bool Save(std::string filePath, std::string fileChanges) = 0;
    virtual std::string Get(std::string filePath) = 0;

    virtial ~IFileHolder()
    {
    }

protected:
    SaveChanges m_changesSavior;
    ReadFile m_fileReader;
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_
