#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor.h"
#include "ace/Signal.h"

#include "handler.h"

ACE_Reactor *g_reactor;

typedef ACE_Acceptor<Logging_Handler, ACE_SOCK_ACCEPTOR> Logging_Acceptor;

// sig_atomic_t - An integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts made by signals.
static sig_atomic_t finished = 0;
extern "C" void handler (int)
{
    finished = 1;
    g_reactor->notify();
}

static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int main(int argc, char* argv[])
{
    // create one reactor
    ACE_NEW_RETURN(g_reactor, ACE_Reactor, -1);

    // create logging acceptor
    Logging_Acceptor peer_acceptor;

    if (peer_acceptor.open(ACE_INET_Addr(PORT), g_reactor) == -1) {
        ACE_ERROR_RETURN((LM_ERROR, "%p\n", "Failed to open peer acceptor"), -1);
    }

    // signal handler
    ACE_Sig_Action sa((ACE_SignalHandler)handler, SIGINT);
    ACE_UNUSED_ARG (sa);

    ACE_DEBUG((LM_DEBUG, "(%P|%t)starting up server logging daemon\n"));

    while(finished == 0) {
        g_reactor->handle_events();
    }

    // close the socket
    peer_acceptor.close();

    delete g_reactor;

    ACE_DEBUG((LM_DEBUG, "(%P|%t)shutting down server logging daemon"));

    return 0;
}
