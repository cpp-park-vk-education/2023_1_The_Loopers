#include "file_holder.h"


namespace inklink_file_holder
{
bool FileHolder::Save(const std::filesystem::path& filePath, const std::string& fileChanges) const
{
    return m_changesSaver.SaveFile(filePath, fileChanges);
}

ReadFileResult FileHolder::Get(const std::filesystem::path& filePath) const
{
    return m_fileReader.GetFile(filePath);
}
};
}  // namespace inklink_file_holder
