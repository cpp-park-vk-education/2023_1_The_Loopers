#include "json_data.h"

#include <nlohmann/json.hpp>

namespace inklink::serializer
{
std::string JsonData::SerializeAsString() const
{
    return m_data.dump();
}

void JsonData::ParseString(const std::string& jsonString)
{
    m_data = nlohmann::json::parse(jsonString);
}

std::string& JsonData::AsString(const std::string& key)
{
    return m_data.at(key).get_ref<std::string&>();
}

const std::string& JsonData::AsString(const std::string& key) const
{
    return m_data.at(key).get_ref<const std::string&>();
}

int& JsonData::AsInt(const std::string& key)
{
    return m_data.at(key).get_ref<int&>();
}

const int& JsonData::AsInt(const std::string& key) const
{
    return m_data.at(key).get_ref<const int&>();
}

double& JsonData::AsDouble(const std::string& key)
{
    return m_data.at(key).get_ref<double&>();
}

const double& JsonData::AsDouble(const std::string& key) const
{
    return m_data.at(key).get_ref<const double&>();
}

bool JsonData::Has(const std::string& key) const
{
    return m_data.contains(key);
}

void JsonData::Clear()
{
    m_data.clear();
}

IData& JsonData::operator[](const std::string& key) noexcept
{
    return m_data[key];
}

const IData& operator[](const std::string& key) const noexcept
{
    return m_data[key];
}

IData& At(const std::string& key) noexcept
{
    return m_data.At(key);
}

const IData& At(const std::string& key) const noexcept
{
    return m_data.At(key);
}
} // namespace inklink::serializer
