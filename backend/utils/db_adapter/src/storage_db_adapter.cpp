#include "storage_db_adapter.h"

#include <pqxx/pqxx> 

#include <string>


namespace inklink::db_adapter
{
void StorageDbAdapter::Connect(const std::string& connectionString) 
{
    m_connection(connectionString.c_str());
    return m_connection.is_open();
}

void StorageDbAdapter::Insert(const std::string& request)
{
    pqxx::work inserter(m_connection);
    inserter.exec(request);

    inserter.commit();
}

std::string StorageDbAdapter::Select(const std::string& request) 
{
    pqxx::work selector(m_connection);
    std::string result;

    pqxx::result response = selector.exec(request);

    for (const auto& iterator: response)
    {
        result = result + response[iterator][0] + "\n";
    }

    return result;
}

void StorageDbAdapter::Update(const std::string& request)
{
    pqxx::work updator(m_connection);
    updator.exec(request);

    updator.commit();
}

void StorageDbAdapter::Delete(const std::string& request)
{
    pqxx::work deletor(m_connection);
    deletor.exec(request);

    deletor.commit();
}
} // namespace inklink::db_adapter
