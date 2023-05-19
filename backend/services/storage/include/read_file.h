#ifndef _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_

#include <string>
#include <experimental/filesystem>


namespace inklink_file_holder
{
namespace filesystem = std::experimental::filesystem;

class ReadFile
{
public:
    const std::string GetFile(const filesystem::path& filePath);
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
