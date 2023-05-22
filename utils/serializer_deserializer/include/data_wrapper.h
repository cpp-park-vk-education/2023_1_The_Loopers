#pragma once

#include <memory>
#include <string>
#include <vector>

namespace inklink::serializer
{
class IData;

class DataWrapper
{
public:
    DataWrapper() = default;
    ~DataWrapper() = default;

    [[nodiscard]] std::string SerializeAsString() const;
    void ParseString(const std::string&);

    [[nodiscard]] std::string& AsString(const std::string&);
    [[nodiscard]] const std::string& AsString(const std::string&) const;
    [[nodiscard]] int& AsInt(const std::string&);
    [[nodiscard]] const int& AsInt(const std::string&) const;
    [[nodiscard]] double& AsDouble(const std::string&);
    [[nodiscard]] const double& AsDouble(const std::string&) const;

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
    mutable std::vector<std::unique_ptr<DataWrapper>> m_buffForValidReferences; // TODO (a.novak) bad design
    std::unique_ptr<IData> m_pData;
};
} // namespace inklink::serializer
