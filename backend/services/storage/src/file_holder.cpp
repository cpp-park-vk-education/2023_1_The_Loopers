#include "file_holder.h"


namespace inklink_file_holder
{
bool FileHolder::Save(const std::filesystem::path& filePath, const std::string& fileChanges) const
{
    return m_changesSavior.SaveFile(filePath, fileChanges);
}

std::string FileHolder::Get(const std::filesystem::path& filePath) const
{
    return m_fileReader.GetFile(filePath);
}
};
}  // namespace inklink_file_holder
