#include "data_container.h"

#include <exception>
#include <stdexcept>

namespace
{
[[nodiscard]] std::string OutOfRangeMessage(const std::string& field)
{
    return std::string("Field '") + field + std::string("' does not exist.");
}
} // namespace

namespace inklink::serializer
{
DataContainer& DataContainer::operator=(int value)
{
    m_data = value;
    return *this;
}

DataContainer& DataContainer::operator=(double value)
{
    m_data = value;
    return *this;
}

DataContainer& DataContainer::operator=(std::string value)
{
    m_data = std::move(value);
    return *this;
}

DataContainer& DataContainer::operator=(std::vector<DataContainer> value)
{
    m_data = std::move(value);
    return *this;
}

DataContainer& DataContainer::CreateArray()
{
    m_data = std::vector<DataContainer>();
    return *this;
}

std::string& DataContainer::AsString()
{
    return std::get<std::string>(m_data);
}

const std::string& DataContainer::AsString() const
{
    return std::get<std::string>(m_data);
}

bool DataContainer::IsString() const
{
    return std::holds_alternative<std::string>(m_data);
}

int& DataContainer::AsInt()
{
    return std::get<int>(m_data);
}

const int& DataContainer::AsInt() const
{
    return std::get<int>(m_data);
}

bool DataContainer::IsInt() const
{
    return std::holds_alternative<int>(m_data);
}

double& DataContainer::AsDouble()
{
    return std::get<double>(m_data);
}

const double& DataContainer::AsDouble() const
{
    return std::get<double>(m_data);
}

bool DataContainer::IsDouble() const
{
    return std::holds_alternative<double>(m_data);
}

std::vector<DataContainer>& DataContainer::AsArray()
{
    return std::get<std::vector<DataContainer>>(m_data);
}

const std::vector<DataContainer>& DataContainer::AsArray() const
{
    return std::get<std::vector<DataContainer>>(m_data);
}

bool DataContainer::IsArray() const
{
    return std::holds_alternative<std::vector<DataContainer>>(m_data);
}

DataContainer::ObjectsContainer& DataContainer::AsObjectsContainer()
{
    return std::get<ObjectsContainer>(m_data);
}

const DataContainer::ObjectsContainer& DataContainer::AsObjectsContainer() const
{
    return std::get<ObjectsContainer>(m_data);
}

bool DataContainer::IsObjectsContainer() const
{
    return std::holds_alternative<ObjectsContainer>(m_data);
}

std::string& DataContainer::AsString(const std::string& field)
{
    return std::get<ObjectsContainer>(m_data).at(field).AsString();
}

const std::string& DataContainer::AsString(const std::string& field) const
{
    return std::get<ObjectsContainer>(m_data).at(field).AsString();
}

bool DataContainer::IsString(const std::string& field) const
{
    const auto& map = std::get<ObjectsContainer>(m_data);
    if (!map.contains(field)) [[unlikely]]
    {
        throw std::out_of_range(OutOfRangeMessage(field));
    }
    return map.at(field).IsString();
}

int& DataContainer::AsInt(const std::string& field)
{
    return std::get<ObjectsContainer>(m_data).at(field).AsInt();
}

const int& DataContainer::AsInt(const std::string& field) const
{
    return std::get<ObjectsContainer>(m_data).at(field).AsInt();
}

bool DataContainer::IsInt(const std::string& field) const
{
    const auto& map = std::get<ObjectsContainer>(m_data);
    if (!map.contains(field)) [[unlikely]]
    {
        throw std::out_of_range(OutOfRangeMessage(field));
    }
    return map.at(field).IsInt();
}

double& DataContainer::AsDouble(const std::string& field)
{
    return std::get<ObjectsContainer>(m_data).at(field).AsDouble();
}

const double& DataContainer::AsDouble(const std::string& field) const
{
    return std::get<ObjectsContainer>(m_data).at(field).AsDouble();
}

bool DataContainer::IsDouble(const std::string& field) const
{
    const auto& map = std::get<ObjectsContainer>(m_data);
    if (!map.contains(field)) [[unlikely]]
    {
        throw std::out_of_range(OutOfRangeMessage(field));
    }
    return map.at(field).IsDouble();
}

std::vector<DataContainer>& DataContainer::AsArray(const std::string& field)
{
    return std::get<ObjectsContainer>(m_data).at(field).AsArray();
}

const std::vector<DataContainer>& DataContainer::AsArray(const std::string& field) const
{
    return std::get<ObjectsContainer>(m_data).at(field).AsArray();
}

bool DataContainer::IsArray(const std::string& field) const
{
    const auto& map = std::get<ObjectsContainer>(m_data);
    if (!map.contains(field)) [[unlikely]]
    {
        throw std::out_of_range(OutOfRangeMessage(field));
    }
    return map.at(field).IsArray();
}

DataContainer::ObjectsContainer& DataContainer::AsObjectsContainer(const std::string& field)
{
    return std::get<ObjectsContainer>(m_data).at(field).AsObjectsContainer();
}

const DataContainer::ObjectsContainer& DataContainer::AsObjectsContainer(const std::string& field) const
{
    return std::get<ObjectsContainer>(m_data).at(field).AsObjectsContainer();
}

bool DataContainer::IsObjectsContainer(const std::string& field) const
{
    const auto& map = std::get<ObjectsContainer>(m_data);
    if (!map.contains(field)) [[unlikely]]
    {
        throw std::out_of_range(OutOfRangeMessage(field));
    }
    return map.at(field).IsObjectsContainer();
}

bool DataContainer::Has(const std::string& field) const
{
    const auto& map = std::get<ObjectsContainer>(m_data);
    return map.contains(field);
}

void DataContainer::Clear()
{
    m_data = CellType{};
}

DataContainer& DataContainer::operator[](const std::string& field) noexcept
{
    try
    {
        return std::get<ObjectsContainer>(m_data)[field];
    }
    catch (const std::bad_variant_access&)
    {
        return *this;
    }
}

const DataContainer& DataContainer::operator[](const std::string& field) const noexcept
{
    try
    {
        return std::get<ObjectsContainer>(m_data).at(field);
    }
    catch (...) // const std::bad_variant_access& and from at
    {
        return *this;
    }
}

DataContainer& DataContainer::At(const std::string& field)
{
    return std::get<ObjectsContainer>(m_data).at(field);
}

const DataContainer& DataContainer::At(const std::string& field) const
{
    return std::get<ObjectsContainer>(m_data).at(field);
}

DataContainer::CellTypeEnum DataContainer::GetCellType(const std::string& field) const
{
    const auto& map = std::get<ObjectsContainer>(m_data);
    if (map.contains(field)) [[likely]]
    {
        const auto& cell = map.at(field).m_data;
        if (std::holds_alternative<ObjectsContainer>(cell))
        {
            return CellTypeEnum::kObjectsContainer;
        }
        if (std::holds_alternative<int>(cell))
        {
            return CellTypeEnum::kInt;
        }
        if (std::holds_alternative<double>(cell))
        {
            return CellTypeEnum::kDouble;
        }
        if (std::holds_alternative<std::string>(cell))
        {
            return CellTypeEnum::kString;
        }
        if (std::holds_alternative<std::vector<DataContainer>>(cell))
        {
            return CellTypeEnum::kArray;
        }
    }
    throw std::out_of_range(OutOfRangeMessage(field));
}
} // namespace inklink::serializer
