#include <string>
#include "file_holder.h"


namespace inklink_file_holder
{
bool FileHolder::Save(std::string filePath, std::string fileChanges)
{
    if (m_changesSavior.SaveFile(filePath, fileChanges))
    {
        return true;
    }
    return false;
}

std::string FileHolder::Get(std::string filePath) override
{
    return m_fileReader.GetFile(filePath);
}
};
}  // namespace inklink_file_holder