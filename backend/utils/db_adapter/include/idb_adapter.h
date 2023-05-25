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

public:
    virtual ~DbAdapterBase() = default;

    virtual void Connect(const std::string& connectionString) = 0;

    template<typename... Arguments>
    virtual void Insert(const std::string& request, const Arguments&... arguments) const = 0;

    template <typename... Arguments>
    virtual void Update(const std::string& request, const Arguments&... arguments) const = 0;

    virtual void Delete(const std::string& request) const = 0;

    template <typename... Arguments>
    [[nodiscard]] virtual DbTable Select(const std::string& request, const Arguments&... arguments) const = 0;
};
} // namespace inklink::db_adapter
