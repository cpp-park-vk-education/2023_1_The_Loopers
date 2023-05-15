#ifndef _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_

#include <string>

namespace inklink_file_holder
{
class SaveChanges
{
public:
    bool SaveFile(const std::string& filePath, const std::string& changes);
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_SAVE_CHANGES_H_
