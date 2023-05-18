#ifndef _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_

#include "ifile_holder.h"

#include <string>

namespace inklink_file_holder
{
class FileHolder : public IFileHolder
{
public:
    bool Save(std::string filePath, std::string fileChanges) override;
    const std::string Get(const std::string& filePath) override;

    virtual ~FileHolder() = default;

protected:
    SaveChanges m_changesSavior;
    ReadFile m_fileReader;
};
} // namespace inklink_file_holder
#endif // _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_
