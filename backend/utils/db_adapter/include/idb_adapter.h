#pragma once

#include <string>
#include <vector>


namespace inklink::db_adapter
{
class DbAdapterBase
{
public:
    using DbRow = std::vector<std::string>;
    using DbTable = std::vector<DbRow>;

    virtual ~DbAdapterBase() = default;

    virtual void Connect(const std::string& connectionString) = 0;

    virtual void Insert(const std::string& request, const std::string& argument1, const std::string& argument2,
                        const std::string& argument3, const std::string& argument4) const = 0;

    virtual void Insert(const std::string& request, const std::string& argument1, const std::string& argument2,
                        const std::string& argument3) const = 0;

    virtual void Update(const std::string& request, const std::string& argument1,
                        const std::string& argument2) const = 0;

    virtual void Delete(const std::string& request) const = 0;

    [[nodiscard]] virtual DbTable Select(const std::string& request, const std::string& argument) const = 0;

    [[nodiscard]] virtual DbTable Select(const std::string& request, const std::string& argument1,
                                         const std::string& argument2) const = 0;

    [[nodiscard]] virtual DbTable Select(const std::string& request, const std::string& argument1,
                                         const std::string& argument2, const std::string& argument3) const = 0;
};
} // namespace inklink::db_adapter
