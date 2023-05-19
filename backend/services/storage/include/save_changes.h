#ifndef _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_

#include <string>
#include <experimental/filesystem>


namespace inklink_file_holder
{
namespace filesystem = std::experimental::filesystem;

class SaveChanges
{
public:
    bool SaveFile(const filesystem::path& filePath, const std::string& changes);
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_
