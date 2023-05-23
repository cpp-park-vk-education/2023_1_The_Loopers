#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace inklink::serializer
{
class DataContainer
{
public:
    using ObjectsContainer = std::unordered_map<std::string, DataContainer>;

public:
    DataContainer& operator=(int);
    DataContainer& operator=(double);
    DataContainer& operator=(std::string);
    DataContainer& operator=(std::vector<DataContainer>);
    DataContainer& CreateArray();

    // /*implicit*/ DataContainer(nlohmann::json&&);

    // [[nodiscard]] std::string SerializeAsString() const;
    // void ParseString(const std::string&);

    // for self
    [[nodiscard]] std::string& AsString();
    [[nodiscard]] const std::string& AsString() const;
    [[nodiscard]] bool IsString() const;

    [[nodiscard]] int& AsInt();
    [[nodiscard]] const int& AsInt() const;
    [[nodiscard]] bool IsInt() const;

    [[nodiscard]] double& AsDouble();
    [[nodiscard]] const double& AsDouble() const;
    [[nodiscard]] bool IsDouble() const;

    [[nodiscard]] std::vector<DataContainer>& AsArray();
    [[nodiscard]] const std::vector<DataContainer>& AsArray() const;
    [[nodiscard]] bool IsArray() const;

    [[nodiscard]] ObjectsContainer& AsObjectsContainer();
    [[nodiscard]] const ObjectsContainer& AsObjectsContainer() const;
    [[nodiscard]] bool IsObjectsContainer() const;

    // for childs (if current is ObjectsContainer)
    [[nodiscard]] std::string& AsString(const std::string&);
    [[nodiscard]] const std::string& AsString(const std::string&) const;
    [[nodiscard]] bool IsString(const std::string&) const;

    [[nodiscard]] int& AsInt(const std::string&);
    [[nodiscard]] const int& AsInt(const std::string&) const;
    [[nodiscard]] bool IsInt(const std::string&) const;

    [[nodiscard]] double& AsDouble(const std::string&);
    [[nodiscard]] const double& AsDouble(const std::string&) const;
    [[nodiscard]] bool IsDouble(const std::string&) const;

    [[nodiscard]] std::vector<DataContainer>& AsArray(const std::string&);
    [[nodiscard]] const std::vector<DataContainer>& AsArray(const std::string&) const;
    [[nodiscard]] bool IsArray(const std::string&) const;

    [[nodiscard]] ObjectsContainer& AsObjectsContainer(const std::string&);
    [[nodiscard]] const ObjectsContainer& AsObjectsContainer(const std::string&) const;
    [[nodiscard]] bool IsObjectsContainer(const std::string&) const;

    [[nodiscard]] bool Has(const std::string&) const;

    void Clear();

    /**
     * @brief Accesses or creates IData given field name
     */
    [[nodiscard]] DataContainer& operator[](const std::string&) noexcept;
    [[nodiscard]] const DataContainer& operator[](const std::string&) const noexcept;
    [[nodiscard]] DataContainer& At(const std::string&);
    [[nodiscard]] const DataContainer& At(const std::string&) const;

private:
    using CellType = std::variant<ObjectsContainer, int, double, std::string, std::vector<DataContainer>>;

    enum class CellTypeEnum
    {
        kInt,
        kDouble,
        kString,
        kArray,
        kObjectsContainer
    };

private:
    // [[nodiscard]] nlohmann::json ToJson() const;

    [[nodiscard]] CellTypeEnum GetCellType(const std::string&) const;

private:
    CellType m_data;
};
} // namespace inklink::serializer
