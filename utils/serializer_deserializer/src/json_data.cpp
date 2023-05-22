#include "json_data.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <memory>
#include <new>

namespace inklink::serializer
{
JsonData::JsonData() : m_data{std::make_shared<nlohmann::json>()}
{
}
/*implicit*/ JsonData::JsonData(nlohmann::json& json) noexcept : m_data{&json}
{
}

/*implicit*/ JsonData::JsonData(nlohmann::json&& json) noexcept
        : m_data{std::make_shared<nlohmann::json>(std::move(json))}
{
}

JsonData::JsonData(const JsonData& json) : m_data{std::make_shared<nlohmann::json>(*json.m_data)}
{
}
JsonData::JsonData(JsonData&& json) noexcept : m_data{std::make_shared<nlohmann::json>(std::move(*json.m_data))}
{
}

JsonData& JsonData::operator=(const JsonData& json)
{
    if (&json != this)
    {
        m_data = std::make_shared<nlohmann::json>(*json.m_data);
    }
    return *this;
}

JsonData& JsonData::operator=(JsonData&& json) noexcept
{
    m_data = std::make_shared<nlohmann::json>(std::move(*json.m_data));
    return *this;
}

std::string JsonData::SerializeAsString() const
{
    return m_data->dump();
}

void JsonData::ParseString(const std::string& jsonString)
{
    m_data = std::make_shared<nlohmann::json>(nlohmann::json::parse(jsonString));
}

std::string& JsonData::AsString(const std::string& key)
{
    return m_data->at(key).get_ref<std::string&>();
}

const std::string& JsonData::AsString(const std::string& key) const
{
    return m_data->at(key).get_ref<const std::string&>();
}

int& JsonData::AsInt(const std::string& key)
{
    return m_data->at(key).get_ref<int&>();
}

const int& JsonData::AsInt(const std::string& key) const
{
    return m_data->at(key).get_ref<const int&>();
}

double& JsonData::AsDouble(const std::string& key)
{
    return m_data->at(key).get_ref<double&>();
}

const double& JsonData::AsDouble(const std::string& key) const
{
    return m_data->at(key).get_ref<const double&>();
}

bool JsonData::Has(const std::string& key) const
{
    return m_data->contains(key);
}

void JsonData::Clear()
{
    m_data->clear();
}

IData* JsonData::operator[](const std::string& key) noexcept
{
    try
    {
        return new JsonData{m_data->operator[](key)};
    }
    catch (const std::bad_alloc&) // TODO (a.novak) may be other exceptions
    {
        return nullptr;
    }
}

const IData* JsonData::operator[](const std::string& key) const noexcept
{
    try
    {
        return new JsonData{m_data->operator[](key)};
    }
    catch (const std::bad_alloc&) // TODO (a.novak) may be other exceptions
    {
        return nullptr;
    }
}

IData* JsonData::At(const std::string& key)
{
    return new JsonData{m_data->at(key)};
}

const IData* JsonData::At(const std::string& key) const
{
    return new JsonData{m_data->at(key)};
}
} // namespace inklink::serializer
