#include <fstream>
#include <string>
#include "read_file.h"

namespace inklink_file_holder
{
std::string ReadFile::GetFile(const std::string& filePath)
{
    std::ifstream fileRead(filePath);
    std::string stringFile("");
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
