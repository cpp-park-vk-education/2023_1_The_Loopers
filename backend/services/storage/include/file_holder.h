#ifndef _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_

#include "ifile_holder.h"


namespace inklink_file_holder
{
class FileHolder final: public IFileHolder
{
public:
    [[nodiscard]] bool Save(const std::filesystem::path& filePath, const std::string& fileChanges) const override;

    [[nodiscard]] ReadFileResult Get(const std::filesystem::path& filePath) const override;
};
} // namespace inklink_file_holder

#endif // _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_
