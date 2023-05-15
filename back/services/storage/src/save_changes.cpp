#include <fstream>
#include <string>
#include "save_changes.h"

namespace inklink_file_holder
{
bool SaveChanges::Save(const std::string& filePath, const std::string& changes)
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