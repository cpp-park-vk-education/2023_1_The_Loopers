#ifndef _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
#define _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_

#include <string>


namespace inklink_file_holder
{
class ReadFile
{
public:
    std::string GetFile(const std::string& filePath);
};
}  // namespace inklink_file_holder
#endif  // _BACK_SERVICES_STORAGE_INCLUDE_READ_FILE_H_
