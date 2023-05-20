#include "read_file.h"

#include <fstream>
#include <string>
#include <sstream>
#include <experimental/filesystem>


namespace inklink_file_holder
{
using filesystem::path = std::experimental::filesystem::path;

const std::string ReadFile::GetFile(const filesystem::path& filePath) noexcept
{
    std::ifstream fileRead(filePath);
    std::string stringFile{""};
    std::stringstream buffer;

    if (!fileRead.is_open())
    {
        return "Uncorrect file path";
    }

    buffer << fileRead.rdbuf();
    stringFile = buffer;

    return stringFile;
}
}  // namespace inklink_file_holder
