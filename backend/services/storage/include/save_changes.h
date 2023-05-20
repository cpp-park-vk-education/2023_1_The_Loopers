#ifndef _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_

#include <string>
#include <experimental/filesystem>


namespace inklink_file_holder
{
using filesystem::path = std::experimental::filesystem::path;

class SaveChanges final
{
public:
    [[nodiscard]] bool SaveFile(const filesystem::path& filePath, const std::string& changes) noexcept;
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_
