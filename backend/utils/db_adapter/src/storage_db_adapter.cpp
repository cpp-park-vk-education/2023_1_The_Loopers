#include "storage_db_adapter.h"
#include "table_after_select.h"

#include <pqxx/pqxx> 

#include <string>
#include <vector>


namespace inklink::db_adapter
{
using namespace data_types;


void StorageDbAdapter::Connect(const std::string& connectionString)
{
    m_connection = std::move(pqxx::connection(connectionString));
}

void StorageDbAdapter::Insert(const std::string& request) const
{
    pqxx::work inserter(m_connection);
    inserter.exec(request);

    inserter.commit();
}

TableAfterSelect StorageDbAdapter::Select(const std::string& request) const
{
    pqxx::work selector(m_connection);
    TableAfterSelect result;

    pqxx::result response = selector.exec(request);

    for (auto row: response)
    {
        std::vector<std::string> currentRow;

        for (auto field : row)
        {
            currentRow.push_back(field.as<std::string>());
        }

        result.PushBackRow(currentRow);
    }

    return result;
}

void StorageDbAdapter::Update(const std::string& request) const
{
    pqxx::work updator(m_connection);
    updator.exec(request);

    updator.commit();
}

void StorageDbAdapter::Delete(const std::string& request) const
{
    pqxx::work deletor(m_connection);
    deletor.exec(request);

    deletor.commit();
}
} // namespace inklink::db_adapter
