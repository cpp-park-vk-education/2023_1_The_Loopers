#pragma once

#include <string>

namespace inklink::serializer
{
class IData
{
public:
    using IDataArray = std::vector<std::shared_ptr<IData>>;

    virtual ~IData() = default;

    [[nodiscard]] virtual std::string SerializeAsString() const = 0;
    virtual void ParseString(const std::string&) = 0;

    [[nodiscard]] virtual std::string& AsString(const std::string&) = 0;
    [[nodiscard]] virtual const std::string& AsString(const std::string&) const = 0;
    [[nodiscard]] virtual bool IsString(const std::string&) const = 0;

    [[nodiscard]] virtual int& AsInt(const std::string&) = 0;
    [[nodiscard]] virtual const int& AsInt(const std::string&) const = 0;
    [[nodiscard]] virtual bool IsInt(const std::string&) const = 0;

    [[nodiscard]] virtual double& AsDouble(const std::string&) = 0;
    [[nodiscard]] virtual const double& AsDouble(const std::string&) const = 0;
    [[nodiscard]] virtual bool IsDouble(const std::string&) const = 0;

    [[nodiscard]] virtual IDataArray& AsArray(const std::string&) = 0;
    [[nodiscard]] virtual const IDataArray& AsArray(const std::string&) const = 0;
    [[nodiscard]] virtual bool IsArray(const std::string&) const = 0;

    [[nodiscard]] virtual bool Has(const std::string&) const = 0;

    virtual void Clear() = 0;

    /**
     * @brief Accesses or creates IData given field name
     */
    [[nodiscard]] virtual IData& operator[](const std::string&) noexcept = 0;
    [[nodiscard]] virtual const IData& operator[](const std::string&) const noexcept = 0;
    [[nodiscard]] virtual IData& At(const std::string&) = 0;
    [[nodiscard]] virtual const IData& At(const std::string&) const = 0;
};
} // namespace inklink::serializer
