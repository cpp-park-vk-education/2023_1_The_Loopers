#pragma once

#include "idata.h"

#include <memory.h>

#include <nlohmann/json.hpp>

#include <memory>
#include <string>

namespace inklink::serializer
{

template <class T>
class NestedSharedPtr
{
public:
    // ideally I want to check if it is inside object or not (it'll work for classes, that are stored sequentially), but
    // for now I won't
    NestedSharedPtr() = default;
    // TODO
private:
    T* m_data;
    std::shared_ptr<T> m_rootObj; // so that object with its subparts won't destruct
};

class JsonData final : public IData
{
public:
    JsonData() noexcept = default;
    explicit JsonData() noexcept;

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
    IData& operator[](const std::string&) noexcept override;
    const IData& operator[](const std::string&) const noexcept override;
    IData& At(const std::string&) override;
    const IData& At(const std::string&) const override;

private:
    nlohmann::json m_data;
    // NestedSharedPtr<nlohmann::json> m_data
};
} // namespace inklink::serializer
