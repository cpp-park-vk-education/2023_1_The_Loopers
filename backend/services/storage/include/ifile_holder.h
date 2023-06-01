#ifndef _BACK_SERVICES_STORAGE_INCLUDE_IFILE_HOLDER_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_IFILE_HOLDER_H_

#include "read_file.h"
#include "save_changes.h"

namespace inklink::file_holder
{
class IFileHolder
{
public:
    using ReadFileResult = std::tuple<std::string, bool>;

public:
    [[nodiscard]] virtual bool Save(const std::filesystem::path& filePath, const std::string& fileChanges) const = 0;

    [[nodiscard]] virtual ReadFileResult Get(const std::filesystem::path& filePath) const = 0;

    virtual ~IFileHolder() = default;

protected:
    FileChangesSaver m_changesSaver;
    FileReader m_fileReader;
};
} // namespace inklink::file_holder

#endif // _BACK_SERVICES_STORAGE_INCLUDE_IFILE_HOLDER_H_