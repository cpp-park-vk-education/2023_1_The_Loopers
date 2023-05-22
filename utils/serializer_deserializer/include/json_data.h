#pragma once

#include "idata.h"

#include <memory.h>

#include <nlohmann/json.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace inklink::serializer
{

// TODO (a.novak) consider saving everything by ourself (using unordered_map<string, JsonData>) and calling
// nlohmann::json only for serialization, parsing
class JsonData final : public IData
{
public:
    JsonData() = default;
    ~JsonData() override = default;

    JsonData(const JsonData&);
    JsonData(JsonData&&) noexcept;

    JsonData& operator=(const JsonData&);
    JsonData& operator=(JsonData&&) noexcept;

    /*implicit*/ JsonData(nlohmann::json&&);

    [[nodiscard]] std::string SerializeAsString() const override;
    void ParseString(const std::string&) override;

    [[nodiscard]] std::string& AsString(const std::string&) override;
    [[nodiscard]] const std::string& AsString(const std::string&) const override;
    [[nodiscard]] bool IsString(const std::string&) const override;

    [[nodiscard]] int& AsInt(const std::string&) override;
    [[nodiscard]] const int& AsInt(const std::string&) const override;
    [[nodiscard]] bool IsInt(const std::string&) const override;

    [[nodiscard]] double& AsDouble(const std::string&) override;
    [[nodiscard]] const double& AsDouble(const std::string&) const override;
    [[nodiscard]] bool IsDouble(const std::string&) const override;

    [[nodiscard]] IDataArray& AsArray(const std::string&) override;
    [[nodiscard]] const IDataArray& AsArray(const std::string&) const override;
    [[nodiscard]] bool IsArray(const std::string&) const override;

    [[nodiscard]] bool Has(const std::string&) const override;

    void Clear() override;

    /**
     * @brief Accesses or creates IData given field name
     */
    [[nodiscard]] IData& operator[](const std::string&) noexcept override;
    [[nodiscard]] const IData& operator[](const std::string&) const noexcept override;
    [[nodiscard]] IData& At(const std::string&) override;
    [[nodiscard]] const IData& At(const std::string&) const override;

private:
    using CellType =
            std::variant<int, double, std::string, std::vector<std::shared_ptr<JsonData>>, std::shared_ptr<JsonData>>;

    enum class CellTypeEnum
    {
        kInt,
        kDouble,
        kString,
        kArray,
        kJsonData
    };
    [[nodiscard]] nlohmann::json ToJson() const;

    [[nodiscard]] CellTypeEnum GetCellType(const std::string&) const;
    [[nodiscard]] bool IsJsonData(const std::string&) const;

    std::unordered_map<std::string, CellType> m_data;
};
} // namespace inklink::serializer
