#include "read_file.h"

#include <fstream>
#include <sstream>


namespace inklink_file_holder
{
ReadFileResult FileReader::GetFile(const std::filesystem::path& filePath) const noexcept
{
    std::ifstream fileRead(filePath);
    std::stringstream stringFile;

    if (!fileRead.is_open())
    {
        return {stringFile, false};
    }

    stringFile << fileRead.rdbuf();

    return {stringFile, true};
}
}  // namespace inklink_file_holder
