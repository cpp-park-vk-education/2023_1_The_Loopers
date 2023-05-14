#ifndef _SAVE_CHANGES_H_
#define _SAVE_CHANGES_H_

#include <string>


class SaveChanges
{
public:
    bool Save(std::string filePath, std::string changes);
};

#endif  // _SAVE_CHANGES_H_