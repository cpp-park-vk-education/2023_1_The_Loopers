#include "file_holder.h"

#include <string>

namespace inklink_file_holder
{
bool FileHolder::Save(std::string filePath, std::string fileChanges)
{
    return m_changesSavior.SaveFile(filePath, fileChanges);
}

const std::string FileHolder::Get(const std::string& filePath) override
{
    return m_fileReader.GetFile(filePath);
}
};
}  // namespace inklink_file_holder
