#include "ace/Reactor.h"

#include "Server.h"

namespace ACE_Server
{

int Server::svc(void)
{
    int result = ACE_Reactor::instance()->run_reactor_event_loop();

    if (result == -1) return -1;

    return 0;
}

}

