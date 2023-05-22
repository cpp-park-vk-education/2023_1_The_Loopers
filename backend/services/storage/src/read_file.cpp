#include "read_file.h"

#include <fstream>
#include <sstream>


namespace inklink_file_holder
{
std::string ReadFile::GetFile(const std::filesystem::path& filePath) const noexcept
{
    std::ifstream fileRead(filePath);
    std::stringstream stringFile;

    if (!fileRead.is_open())
    {
        return "Uncorrect file path";
    }

    stringFile << fileRead.rdbuf();

    return stringFile;
}
}  // namespace inklink_file_holder
