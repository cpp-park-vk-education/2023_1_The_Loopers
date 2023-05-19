#include "save_changes.h"

#include <fstream>
#include <string>
#include <experimental/filesystem>



namespace inklink_file_holder
{
using filesystem::path = std::experimental::filesystem::path;

bool SaveChanges::SaveFile(const filesystem::path& filePath, const std::string& changes)
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
