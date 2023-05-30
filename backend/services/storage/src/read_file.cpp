#include "read_file.h"

#include <fstream>
#include <sstream>

namespace inklink::file_holder
{
FileReader::ReadFileResult FileReader::GetFile(const std::filesystem::path& filePath) const noexcept
{
    std::ifstream fileRead(filePath);
    std::stringstream stringFile;

    if (!fileRead.is_open())
    {
        return {stringFile.str(), false};
    }

    stringFile << fileRead.rdbuf();
    
    return {stringFile.str(), true};
}
}  // namespace inklink::file_holder
