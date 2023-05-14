#include <string>
#include "file_holder.h"


namespace inklink_file_holder
{
class FileHolder : public IFileHolder
{
    bool Save(std::string filePath, std::string fileChanges) override
    {
        if (m_changesSavior.Save(filePath, fileChanges))
        {
            return true;
        }
        return false;
    }


    bool Delete(std::string filePath) override
    {
        if (m_fileDeletor.Delete(filePath))
        {
            return true;
        }
        return false;
    }


    std::string Read(std::string filePath) override
    {
        return m_fileReader.Get(filePath);
    }


    ~FileHolder() 
    {
    }
};

}