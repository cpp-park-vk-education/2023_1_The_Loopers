#include "json_data.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <memory>
#include <new>
#include <stdexcept>

namespace inklink::serializer
{
std::string JsonData::SerializeAsString() const
{
    const nlohmann::json jsonData = ToJson();
    return jsonData.dump(); // Convert JSON to string
}

void JsonData::ParseString(const std::string& jsonString)
{
    try
    {
        nlohmann::json jsonData = nlohmann::json::parse(jsonString); // Parse JSON from string
        *this = JsonData(std::move(jsonData));                       // Construct JsonData object from parsed JSON
    }
    catch (const nlohmann::json::exception& e)
    {
        throw std::logic_error(e.what());
    }
}

std::string& JsonData::AsString(const std::string& key)
{
    return std::get<std::string>(m_data.at(key));
}

const std::string& JsonData::AsString(const std::string& key) const
{
    return std::get<std::string>(m_data.at(key));
}

bool JsonData::IsString(const std::string& key) const
{
    return std::holds_alternative<std::string>(m_data.at(key));
}

int& JsonData::AsInt(const std::string& key)
{
    return std::get<int>(m_data.at(key));
}

const int& JsonData::AsInt(const std::string& key) const
{
    return std::get<int>(m_data.at(key));
}

bool JsonData::IsInt(const std::string& key) const
{
    return std::holds_alternative<int>(m_data.at(key));
}

double& JsonData::AsDouble(const std::string& key)
{
    return std::get<double>(m_data.at(key));
}

const double& JsonData::AsDouble(const std::string& key) const
{
    return std::get<double>(m_data.at(key));
}

bool JsonData::IsDouble(const std::string& key) const
{
    return std::holds_alternative<double>(m_data.at(key));
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
    try
    {
        return *std::get<std::shared_ptr<JsonData>>(m_data[key]);
    }
    catch (const std::bad_variant_access&)
    {
        return *this;
    }
}

const IData& JsonData::operator[](const std::string& key) const noexcept
{
    try
    {
        return *std::get<std::shared_ptr<JsonData>>(m_data[key]);
    }
    catch (const std::bad_variant_access&)
    {
        return *this;
    }
}

IData& JsonData::At(const std::string& key)
{
    return *std::get<std::shared_ptr<JsonData>>(m_data.at(key));
}

const IData& JsonData::At(const std::string& key) const
{
    return *std::get<std::shared_ptr<JsonData>>(m_data.at(key));
}

JsonData::JsonData(nlohmann::json&& jsonData)
{
    for (const auto& [key, value] : jsonData.items())
    {
        if (value.is_number_integer())
        {
            m_data[key] = value.get<int>();
        }
        else if (value.is_number_float())
        {
            m_data[key] = value.get<double>();
        }
        else if (value.is_string())
        {
            m_data[key] = value.get<std::string>();
        }
        else if (value.is_object())
        {
            m_data[key] = std::make_shared<JsonData>(std::move(value));
        }
    }
}

nlohmann::json JsonData::ToJson() const
{
    nlohmann::json jsonData;

    for (const auto& [key, value] : m_data)
    {
        if (std::holds_alternative<int>(value))
        {
            jsonData[key] = std::get<int>(value);
        }
        else if (std::holds_alternative<double>(value))
        {
            jsonData[key] = std::get<double>(value);
        }
        else if (std::holds_alternative<std::string>(value))
        {
            jsonData[key] = std::get<std::string>(value);
        }
        else if (std::holds_alternative<std::shared_ptr<JsonData>>(value))
        {
            jsonData[key] = std::get<std::shared_ptr<JsonData>>(value)->ToJson();
        }
    }

    return jsonData;
}

JsonData::CellTypeEnum JsonData::GetCellType(const std::string& key) const
{
    if (IsJsonData(key))
    {
        return CellTypeEnum::kJsonData;
    }
    if (IsString(key))
    {
        return CellTypeEnum::kString;
    }
    if (IsDouble(key))
    {
        return CellTypeEnum::kInt;
    }
    if (IsInt(key))
    {
        return CellTypeEnum::kDouble;
    }
}

bool JsonData::IsJsonData(const std::string& key) const
{
    return std::holds_alternative<std::shared_ptr<JsonData>>(m_data.at(key));
}
} // namespace inklink::serializer
