#include "idb_adapter.h"


namespace inklink::db_adapter
{
void DbAdapterBase::Connect(const std::string& connectionString)
{
    static_cast<Implementation*>(this)->Connect(connectionString);
}

std::shared_ptr<Connection> DbAdapterBase::GetConnection()
{
    static_cast<Implementation*>(this)->GetConnection();
}

template <typename... Arguments>
void DbAdapterBase::Insert(const std::string& request, const Arguments&... arguments) const
{
    static_cast<Implementation*>(this)->Insert(request, arguments...);
}

template <typename... Arguments>
void DbAdapterBase::Update(const std::string& request, const Arguments&... arguments) const
{
    static_cast<Implementation*>(this)->Update(request, arguments...);
}

template <typename... Arguments>
DbAdapterBase::DbTable DbAdapterBase::Select(const std::string& request, const Arguments&... arguments) const
{
    static_cast<Implementation*>(this)->Select(request, arguments...);
}

void Delete(const std::string& request) const
{
    static_cast<Implementation*>(this)->Delete(request);
}
} // namespace inklink::db_adapter
