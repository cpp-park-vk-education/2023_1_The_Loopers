#include "read_file.h"

#include <fstream>
#include <string>
#include <experimental/filesystem>


namespace inklink_file_holder
{
namespace filesystem = std::experimental::filesystem;

const std::string ReadFile::GetFile(const filesystem::path& filePath)
{
    std::ifstream fileRead(filePath);
    std::string stringFile{""};
    std::string buffer;

    if (!fileRead.is_open())
    {
        return "Uncorrect file path";
    }

    while (fileRead >> buffer)
    {
        stringFile = stringFile + buffer;
    }

    return stringFile;
}
}  // namespace inklink_file_holder
