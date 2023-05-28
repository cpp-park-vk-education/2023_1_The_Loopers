#pragma once

#include <concepts>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace inklink::serializer
{
template <typename T>
concept AllowedSimpleType = std::same_as<T, int> || std::same_as<T, std::string> || std::same_as<T, double>;

template <typename T>
concept AllowedVectorType = std::same_as<T, std::vector<int>> || std::same_as<T, std::vector<std::string>> ||
                            std::same_as<T, std::vector<double>>;

class DataContainer
{
public:
    using ObjectsContainer = std::unordered_map<std::string, std::shared_ptr<DataContainer>>;

public:
    DataContainer() = default;
    ~DataContainer() = default;

    template <AllowedSimpleType T>
    /*implicit*/ DataContainer(T value);
    template <AllowedVectorType T>
    /*implicit*/ DataContainer(T vector);
    /*implicit*/ DataContainer(std::vector<DataContainer> vector);

    DataContainer(const DataContainer&);
    DataContainer(DataContainer&&) noexcept;

    DataContainer& operator=(const DataContainer&);
    DataContainer& operator=(DataContainer&&) noexcept;

    template <AllowedSimpleType T>
    DataContainer& operator=(T value);
    DataContainer& operator=(const char* value);
    template <AllowedVectorType T>
    DataContainer& operator=(T vector);
    DataContainer& operator=(std::vector<DataContainer> vector);
    std::vector<DataContainer>& CreateArray();

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
    [[nodiscard]] CellTypeEnum GetCellType(const std::string&) const;

private:
    CellType m_data;
};

template <AllowedSimpleType T>
inline /*implicit*/ DataContainer::DataContainer(T value) : m_data{std::move(value)}
{
}

template <AllowedVectorType T>
inline /*implicit*/ DataContainer::DataContainer(T vector)
{
    auto& array = CreateArray();
    for (auto& value : vector)
    {
        // will implicitly call constructor DataContainer<AllowedSimpleType> for each value
        array.push_back(std::move(value));
    }
}

inline /*implicit*/ DataContainer::DataContainer(std::vector<DataContainer> vector) : m_data{std::move(vector)}
{
}

template <AllowedSimpleType T>
inline DataContainer& DataContainer::operator=(T value)
{
    m_data = std::move(value);
    return *this;
}

inline DataContainer& DataContainer::operator=(const char* value)
{
    m_data = std::string(value);
    return *this;
}

template <AllowedVectorType T>
inline DataContainer& DataContainer::operator=(T vector)
{
    auto& array = CreateArray();
    for (auto& value : vector)
    {
        // will implicitly call constructor DataContainer<AllowedSimpleType> for each value
        array.push_back(std::move(value));
    }
    return *this;
}
inline DataContainer& DataContainer::operator=(std::vector<DataContainer> vector)
{
    auto& array = CreateArray();
    for (auto& value : vector)
    {
        array.push_back(std::move(value));
    }
    return *this;
}

} // namespace inklink::serializer
