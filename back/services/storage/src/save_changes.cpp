#include <fstream>
#include <string>
#include "save_changes.h"


bool SaveChanges::Save(std::string filePath, std::string changes)
{
    std::ofstream file(filePath, std::ios::app);

    if (!file.is_open())
    {
        return false;
    }


    file << changes;

    return true;
}