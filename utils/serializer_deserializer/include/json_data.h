#pragma once

#include "idata.h"

#include <memory.h>

#include <nlohmann/json.hpp>

#include <memory>
#include <string>

namespace inklink::serializer
{

// TODO (a.novak) consider saving everything by ourself (using unordered_map<string, JsonData>) and calling
// nlohmann::json only for serialization, parsing
class JsonData final : public IData
{
public:
    JsonData();
    /*implicit*/ JsonData(nlohmann::json&) noexcept;
    /*implicit*/ JsonData(nlohmann::json&&) noexcept;

    JsonData(const JsonData&);
    JsonData(JsonData&&) noexcept;

    JsonData& operator=(const JsonData&);
    JsonData& operator=(JsonData&&) noexcept;

    ~JsonData() override = default;

    [[nodiscard]] std::string SerializeAsString() const override;
    void ParseString(const std::string&) override;

    [[nodiscard]] std::string& AsString(const std::string&) override;
    [[nodiscard]] const std::string& AsString(const std::string&) const override;
    [[nodiscard]] int& AsInt(const std::string&) override;
    [[nodiscard]] const int& AsInt(const std::string&) const override;
    [[nodiscard]] double& AsDouble(const std::string&) override;
    [[nodiscard]] const double& AsDouble(const std::string&) const override;

    [[nodiscard]] bool Has(const std::string&) const override;

    void Clear() override;

    /**
     * @brief Accesses or creates IData given field name
     */
    [[nodiscard]] IData* operator[](const std::string&) noexcept override;
    [[nodiscard]] const IData* operator[](const std::string&) const noexcept override;
    [[nodiscard]] IData* At(const std::string&) override;
    [[nodiscard]] const IData* At(const std::string&) const override;

private:
    std::shared_ptr<nlohmann::json> m_data;
    // std::shared_ptr<nlohmann::json> m_rootData; // if in nlohmann json deletion of parent object when childs are in
    // use is possible. But I don't think so
};
} // namespace inklink::serializer
