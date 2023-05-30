#include "file_holder.h"

namespace inklink::file_holder
{
bool FileHolder::Save(const std::filesystem::path& filePath, const std::string& fileChanges) const
{
    return m_changesSaver.SaveFile(filePath, fileChanges);
}

FileHolder::ReadFileResult FileHolder::Get(const std::filesystem::path& filePath) const
{
    return m_fileReader.GetFile(filePath);
}
}  // namespace inklink_file_holder
