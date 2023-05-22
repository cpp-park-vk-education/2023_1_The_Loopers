#pragma once

#include <concepts>
#include <memory>
#include <string>
#include <vector>

namespace inklink::serializer
{
class IData;

template <typename T>
concept DerivedFromIData = std::is_base_of_v<IData, T>;

template <DerivedFromIData T>
class DataWrapper
{
public:
    DataWrapper();

    /*implicit*/ DataWrapper(IData&);
    /*implicit*/ DataWrapper(const IData&);

    DataWrapper(const DataWrapper&);
    DataWrapper(DataWrapper&&) noexcept;

    DataWrapper& operator=(const DataWrapper&);
    DataWrapper& operator=(DataWrapper&&) noexcept;

    ~DataWrapper() = default;

    [[nodiscard]] std::string SerializeAsString() const;
    void ParseString(const std::string&);

    [[nodiscard]] std::string& AsString(const std::string&);
    [[nodiscard]] const std::string& AsString(const std::string&) const;
    [[nodiscard]] bool IsString(const std::string&) const;

    [[nodiscard]] int& AsInt(const std::string&);
    [[nodiscard]] const int& AsInt(const std::string&) const;
    [[nodiscard]] bool IsInt(const std::string&) const;

    [[nodiscard]] double& AsDouble(const std::string&);
    [[nodiscard]] const double& AsDouble(const std::string&) const;
    [[nodiscard]] bool IsDouble(const std::string&) const;

    [[nodiscard]] bool Has(const std::string&) const;

    void Clear();

    /**
     * @brief Accesses or creates IData given field name
     */
    [[nodiscard]] DataWrapper& operator[](const std::string&) noexcept;
    [[nodiscard]] const DataWrapper& operator[](const std::string&) const noexcept;
    [[nodiscard]] DataWrapper& At(const std::string&);
    [[nodiscard]] const DataWrapper& At(const std::string&) const;

private:
    std::unique_ptr<IData> m_pData;
};
} // namespace inklink::serializer
