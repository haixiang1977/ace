#include "ace/Reactor.h"

#include "logging_acceptor.h"

ACE_Reactor *g_reactor;

static const size_t PORT = ACE_DEFAULT_SERVER_PORT; // 20002

int main(int argc, char* argv[])
{
    ACE_NEW_RETURN(g_reactor, ACE_Reactor, 1);

    ACE_INET_Addr addr(PORT);

    Logging_Acceptor *peer_acceptor;

    ACE_NEW_RETURN(peer_acceptor, Logging_Acceptor, 1);

    // link reactor with socket
    if (peer_acceptor->open(addr, g_reactor) == -1) {
        ACE_ERROR_RETURN((LM_ERROR, "Error Opening Acceptor\n"), -1);
    }

    ACE_DEBUG((LM_DEBUG, "(%P|%t) starting up server logging daemon\n"));

    // The reactor's handle_events member function is responsible for looking at all registered objects and invoking an appropriate
    // member function when anything of interest occurs. When an event is processed, the handle_events function returns. In order to 
    // get all events, we embed this in an infinite loop.
    while (1) {
        g_reactor->handle_events();
    }

    return 0;
}

