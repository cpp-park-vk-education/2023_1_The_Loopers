#ifndef _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_

#include "ifile_holder.h"

namespace inklink::file_holder
{
class FileHolder final: public IFileHolder
{
public:
    using ReadFileResult = std::tuple<std::string, bool>;

public:
    [[nodiscard]] bool Save(const std::filesystem::path& filePath, const std::string& fileChanges) const override;

    [[nodiscard]] ReadFileResult Get(const std::filesystem::path& filePath) const override;
};
} // namespace inklink::file_holder

#endif // _BACK_SERVICES_STORAGE_INCLUDE_FILE_HOLDER_H_
