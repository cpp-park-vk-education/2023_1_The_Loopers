#ifndef _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_

#include <filesystem>
#include <string>
#include <tuple>


namespace inklink_file_holder
{
class FileReader final
{
using ReadFileResult = std::tuple<std::string, bool>;

public:
    [[nodiscard]] ReadFileResult GetFile(const std::filesystem::path& filePath) const noexcept;
};
}  // namespace inklink_file_holder

#endif  // _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
