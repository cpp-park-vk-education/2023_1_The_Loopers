#include "json_serializer.h"

#include "data_container.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace
{
using DataContainer = inklink::serializer::DataContainer;

json SerializeDataContainer(const DataContainer& data)
{
    json jsonData;

    if (data.IsInt())
    {
        jsonData = data.AsInt();
    }
    else if (data.IsDouble())
    {
        jsonData = data.AsDouble();
    }
    else if (data.IsString())
    {
        jsonData = data.AsString();
    }
    else if (data.IsArray())
    {
        const auto& arrayData = data.AsArray();
        for (const auto& element : arrayData)
        {
            jsonData.push_back(SerializeDataContainer(element));
        }
    }
    else if (data.IsObjectsContainer())
    {
        const auto& mapData = data.AsObjectsContainer();
        for (const auto& pair : mapData)
        {
            jsonData[pair.first] = SerializeDataContainer(pair.second);
        }
    }

    return jsonData;
}

DataContainer DeserializeDataContainer(const json& jsonData)
{
    DataContainer data;

    if (jsonData.is_number_integer())
    {
        data = jsonData.get<int>();
    }
    else if (jsonData.is_number_float())
    {
        data = jsonData.get<double>();
    }
    else if (jsonData.is_string())
    {
        data = jsonData.get<std::string>();
    }
    else if (jsonData.is_array())
    {
        for (const auto& element : jsonData)
        {
            data.AsArray().push_back(DeserializeDataContainer(element));
        }
    }
    else if (jsonData.is_object())
    {
        for (json::const_iterator it = jsonData.begin(); it != jsonData.end(); ++it)
        {
            data[it.key()] = DeserializeDataContainer(it.value());
        }
    }

    return data;
}
} // namespace

namespace inklink::serializer
{
std::string JsonSerializer::SerializeAsString(const DataContainer& data)
{
    const json jsonData = SerializeDataContainer(data);
    return jsonData.dump();
}

DataContainer JsonSerializer::ParseFromString(const std::string& jsonString)
{
    const json jsonData = json::parse(jsonString);
    return DeserializeDataContainer(jsonData);
}
} // namespace inklink::serializer
