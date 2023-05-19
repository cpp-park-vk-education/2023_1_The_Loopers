#ifndef _BACK_SERVICES_STORAGE_INCLUDE_IFILE_HOLDER_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_IFILE_HOLDER_H_

#include "delete_file.h"
#include "read_file.h"
#include "save_changes.h"

#include <string>
#include <experimental/filesystem>


namespace inklink_file_holder
{
using filesystem::path = std::experimental::filesystem::path;

class IFileHolder
{
public:
    virtual bool Save(const filesystem::path& filePath, std::string fileChanges) = 0;
    virtual const std::string Get(const filesystem::path& filePath) = 0;

    virtual ~IFileHolder() = default;

protected:
    SaveChanges m_changesSavior;
    ReadFile m_fileReader;
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_IFILE_HOLDER_H_
