#pragma once

#include "idb_adapter.h"
#include "iexternal_service_chassiss.h"
#include "iresolvers_factory.h"

#include <map>
#include <memory>
#include <string>

class SimultaneousAccessService
{
public:
    void SetResolversFactory(IResolversFactory*)
    {
    }
    void SetServiceChassis(IExternalServiceChassis*)
    {
    }
    void SetDbAdapter(IDBAdapter*)
    {
    }

    void run(unsigned short port)
    {
    }

private:
    void DoOnConnect()
    {
    }
    void DoOnRead(const string&, IServiceSession*)
    {
    }
    void DoOnWrite()
    {
    }

    std::map<std::chrono::time_point<std::chrono::system_clock>, DrawAction> m_notApplied;
};
