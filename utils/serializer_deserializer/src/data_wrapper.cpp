#include "data_wrapper.h"

#include "idata.h"

namespace inklink::serializer
{
template <DerivedFromIData T>
DataWrapper<T>::DataWrapper() : m_pData{std::make_unique<T>()}
{
}

template <DerivedFromIData T>
/*implicit*/ DataWrapper<T>::DataWrapper(IData& data) : m_pData{std::unique_ptr<IData>(&data)}
{
}

template <DerivedFromIData T>
/*implicit*/ DataWrapper<T>::DataWrapper(const IData& data) : m_pData{std::make_unique<IData>(data)}
{
}

template <DerivedFromIData T>
DataWrapper<T>::DataWrapper(const DataWrapper& other)
{
    m_pData = std::make_unique<IData>(*other.m_pData);
}

template <DerivedFromIData T>
DataWrapper<T>::DataWrapper(DataWrapper&& other) noexcept
{
    m_pData = std::move(other.m_pData);
}

template <DerivedFromIData T>
DataWrapper<T>& DataWrapper<T>::operator=(const DataWrapper& other)
{
    if (this != &other)
    {
        m_pData = std::make_unique<IData>(*other.m_pData);
    }
    return *this;
}

template <DerivedFromIData T>
DataWrapper<T>& DataWrapper<T>::operator=(DataWrapper&& other) noexcept
{
    m_pData = std::move(other.m_pData);
    return *this;
}

template <DerivedFromIData T>
std::string DataWrapper<T>::SerializeAsString() const
{
    return m_pData->SerializeAsString();
}

template <DerivedFromIData T>
void DataWrapper<T>::ParseString(const std::string& data)
{
    m_pData->ParseString(data);
}

template <DerivedFromIData T>
std::string& DataWrapper<T>::AsString(const std::string& field)
{
    return m_pData->AsString(field);
}

template <DerivedFromIData T>
const std::string& DataWrapper<T>::AsString(const std::string& field) const
{
    return m_pData->AsString(field);
}

template <DerivedFromIData T>
bool DataWrapper<T>::IsString(const std::string& field) const
{
    return m_pData->IsString(field);
}

template <DerivedFromIData T>
int& DataWrapper<T>::AsInt(const std::string& field)
{
    return m_pData->AsInt(field);
}

template <DerivedFromIData T>
const int& DataWrapper<T>::AsInt(const std::string& field) const
{
    return m_pData->AsInt(field);
}

template <DerivedFromIData T>
bool DataWrapper<T>::IsInt(const std::string& field) const
{
    return m_pData->IsInt(field);
}

template <DerivedFromIData T>
double& DataWrapper<T>::AsDouble(const std::string& field)
{
    return m_pData->AsDouble(field);
}

template <DerivedFromIData T>
const double& DataWrapper<T>::AsDouble(const std::string& field) const
{
    return m_pData->AsDouble(field);
}

template <DerivedFromIData T>
bool DataWrapper<T>::IsDouble(const std::string& field) const
{
    return m_pData->IsDouble(field);
}

template <DerivedFromIData T>
void DataWrapper<T>::Clear()
{
    m_pData.reset();
}

template <DerivedFromIData T>
DataWrapper<T>& DataWrapper<T>::operator[](const std::string& field) noexcept
{
    return (*m_pData)[field];
}

template <DerivedFromIData T>
const DataWrapper<T>& DataWrapper<T>::operator[](const std::string& field) const noexcept
{
    return (*m_pData)[field];
}

template <DerivedFromIData T>
DataWrapper<T>& DataWrapper<T>::At(const std::string& field)
{
    return m_pData->At(field);
}

template <DerivedFromIData T>
const DataWrapper<T>& DataWrapper<T>::At(const std::string& field) const
{
    return m_pData->At(field);
}
} // namespace inklink::serializer
