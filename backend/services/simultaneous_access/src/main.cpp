#include "simultaneous_access_service.h"

int main()
{
    inklink::service_simultaneous_access::SimultaneousAccessService service;
    return service.Run();
}
