#ifndef _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_

#include <filesystem>
#include <string>


namespace inklink_file_holder
{
class ReadFile final
{
public:
    [[nodiscard]] std::string GetFile(const std::filesystem::path& filePath) const  noexcept;
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
