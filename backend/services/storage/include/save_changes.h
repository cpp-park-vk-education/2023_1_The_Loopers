#ifndef _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_

#include <filesystem>
#include <string>


namespace inklink_file_holder
{
class SaveChanges final
{
public:
    bool SaveFile(const std::filesystem::path& filePath, const std::string& changes) const noexcept;
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_
