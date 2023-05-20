#ifndef _BACK_SERVICES_STORAGE_INCLUDE_IFILE_HOLDER_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_IFILE_HOLDER_H_

#include "delete_file.h"
#include "read_file.h"
#include "save_changes.h"

#include <experimental/filesystem>
#include <string>

namespace inklink:file_holder
{
using filesystem::path = std::experimental::filesystem::path;

class IFileHolder
{
public:
    [[nodiscard]] virtual bool Save(const filesystem::path& filePath, std::string fileChanges) = 0;

    [[nodiscard]] virtual const std::string Get(const filesystem::path& filePath) = 0;

    virtual ~IFileHolder() = default;

protected:
    SaveChanges m_changesSavior;
    ReadFile m_fileReader;
};
} // namespace inklink:file_holder
#endif // _BACK_SERVICES_STORAGE_INCLUDE_IFILE_HOLDER_H_