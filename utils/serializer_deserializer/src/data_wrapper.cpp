#include "data_wrapper.h"

#include "idata.h"

namespace inklink::serializer
{
std::string DataWrapper::SerializeAsString() const
{
    if (m_pData)
    {
        return m_pData->SerializeAsString();
    }
    return "";
}

void DataWrapper::ParseString(const std::string& str)
{
    if (m_pData)
    {
        m_pData->ParseString(str);
    }
}

std::string& DataWrapper::AsString(const std::string& key)
{
    if (m_pData)
    {
        return m_pData->AsString(key);
    }
    throw std::runtime_error("DataWrapper: No data present");
}

const std::string& DataWrapper::AsString(const std::string& key) const
{
    if (m_pData)
    {
        return m_pData->AsString(key);
    }
    throw std::runtime_error("DataWrapper: No data present");
}

int& DataWrapper::AsInt(const std::string& key)
{
    if (m_pData)
    {
        return m_pData->AsInt(key);
    }
    throw std::runtime_error("DataWrapper: No data present");
}

const int& DataWrapper::AsInt(const std::string& key) const
{
    if (m_pData)
    {
        return m_pData->AsInt(key);
    }
    throw std::runtime_error("DataWrapper: No data present");
}

bool DataWrapper::Has(const std::string& key) const
{
    if (m_pData)
    {
        return m_pData->Has(key);
    }
    return false;
}

void DataWrapper::Clear()
{
    if (m_pData)
    {
        m_pData->Clear();
    }
}

DataWrapper& DataWrapper::operator[](const std::string& key) noexcept
{
    IData* element = (*m_pData)[key];
    m_buffForValidReferences.push_back(std::unique_ptr<DataWrapper>(element));
    return m_buffForValidRefernces.back();
}

const DataWrapper& DataWrapper::operator[](const std::string& key) const noexcept
{
    IData* element = (*m_pData)[key];
    m_buffForValidReferences.push_back(std::unique_ptr<DataWrapper>(element));
    return m_buffForValidRefernces.back();
}

DataWrapper& DataWrapper::At(const std::string& key)
{
    try
    {
        if (m_pData)
        {
            IData* element = m_pData->At(key);
            m_buffForValidReferences.push_back(std::unique_ptr<IData>(element));
            return m_buffForValidRefernces.back();
        }
        throw std::runtime_error("DataWrapper: No data present");
    }
    catch (...) // from push_back
    {
    }
}

const DataWrapper& DataWrapper::At(const std::string& key) const
{
    try
    {
        if (m_pData)
        {
            IData* element = m_pData->At(key);
            m_buffForValidReferences.push_back(std::unique_ptr<IData>(element));
            return m_buffForValidRefernces.back();
        }
        throw std::runtime_error("DataWrapper: No data present");
    }
    catch (...) // from push_back
    {
    }
}
} // namespace inklink::serializer
