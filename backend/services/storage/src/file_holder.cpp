#include "file_holder.h"

#include <string>
#include <experimental/filesystem>


namespace inklink_file_holder
{
namespace filesystem = std::experimental::filesystem;

bool FileHolder::Save(const filesystem::path& filePath, std::string fileChanges)
{
    return m_changesSavior.SaveFile(filePath, fileChanges);
}

const std::string FileHolder::Get(const filesystem::path& filePath)
{
    return m_fileReader.GetFile(filePath);
}
};
}  // namespace inklink_file_holder
