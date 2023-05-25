#include "message_broker_service.h"

int main(int argc, char** argv)
{
    inklink::service_message_broker::MessageBrokerService service;
    return service.Run();
}
