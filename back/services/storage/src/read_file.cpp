#include <fstream>
#include <string>
#include "ReadFile.h"


std::string ReadFile::Get(std::string filePath)
{
    std::ifstream fileRead(filePath);
    std::string stringFile("");
    std::string buffer;


    if (!fileRead.is_open())
    {
        return "Uncorrect file path";
    }

    while (!fileRead.eof())
    {
        fileRead >> buffer;
        stringFile = stringFile + buffer;
    }

    return stringFile;
}