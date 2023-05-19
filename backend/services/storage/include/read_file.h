#ifndef _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_

#include <string>
#include <experimental/filesystem>


namespace inklink_file_holder
{
using filesystem::path = std::experimental::filesystem::path;

class ReadFile
{
public:
    [[nodiscard]] const std::string GetFile(const filesystem::path& filePath) noexcept;
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
