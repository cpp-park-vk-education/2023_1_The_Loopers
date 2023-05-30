#include "save_changes.h"

#include <fstream>


namespace inklink_file_holder
{
bool FileChangesSaver::SaveFile(const std::filesystem::path& filePath, const std::string& changes) const noexcept
{
    std::ofstream file(filePath, std::ios::app);

    if (!file.is_open())
    {
        return false;
    }

    file << changes;

    return true;
}
}  // namespace inklink_file_holder
