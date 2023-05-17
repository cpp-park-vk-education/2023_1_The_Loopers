#include <iostream>
#include <string>
#include <pqxx/pqxx> 
#include "idb_adapter.h"

namespace inklink::db_adapter
{
class StorageDbAdapter : public PgSqlIDbAdapter
{
    void Connect(const std::string& connectionString) override
    {
        m_connection(connectionString.c_str());
    }
    void Insert(const std::string& insertPart, const std::string& valuesPart) override
    {
        pqxx::work inserter(m_connection);
        inserter.exec("INSERT INTO " + insertPart + " VALUES ( " + valuesPart + " )");

        inserter.commit();
    }

    std::string Select(const std::string& selectPart, const std::string& fromPart, const std::string& wherePart,
                        const std::string& orderPart) override
    {
        pqxx::work selector(m_connection);
        std::string result("");

        pqxx::result response = selector.exec("SELECT " + selectPart + " FROM " + fromPart + " WHERE " + wherePart);

        for (int i = 0; i < response.size(); ++i)
        {
            result = result + response[i][0] + "\n";
        }

        return result;
    }

    void Update(const std::string& updatePart, const std::string& setPart, const std::stirng wherePart) override
    {
        pqxx::work updator(m_connection);
        updator.exec("UPDATE " + updatePart + " SET " + setPart + " WHERE " + wherePart);

        updator.commit();
    }

    void Delete(const std::string& deletePart, const std::string& fromPart, const std::string& wherePart) override
    {
        pqxx::work deletor(m_connection);
        deletor.exec("DELETE " + deletePart + " FROM " + fromPart + " WHERE " + wherePart);

        deletor.commit();
    }

    ~StorageDbAdapter()
    {
    }
};
} // namespace inklink::db_adapter
