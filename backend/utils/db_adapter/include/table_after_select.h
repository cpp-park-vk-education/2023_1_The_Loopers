#pragma once

#include <vector>
#include <string>


namespace inklink::db_controller
{
struct TableAfterSelect
{
public:
    [[nodiscard]] int TableSize() const
    {
        return table.size();
    }

    [[nodiscard]] std::vector<std::string> operator[](const int iterator) const
    {
        return table[iterator];
    }

    void PushBackRow(std::vector<std::string>& row)
    {
        table.push_back(row);
    }

private:
    std::vector<std::vector<std::string>> table;
};
} // namespace inklink::db_controller