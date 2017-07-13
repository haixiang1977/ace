#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/TP_Reactor.h"
#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/OS.h"

#include "Request_Handler.h"
#include "Server.h"
#include "Constants.h"

using namespace ACE_Server;

int main(int arg, char* argv[])
{
    ACE_TP_Reactor tp_reactor;
    ACE_Reactor reactor(&tp_reactor, 1);
    ACE_Reactor::instance(&reactor, 1);

    ACE_Acceptor<Request_Handler, ACE_SOCK_ACCEPTOR> acceptor;
    ACE_INET_Addr addr(SERVER_PORT_NUM);

    if (acceptor.open(addr) == -1) {
        return -1;
    }

    Server server_tp;

    server_tp.activate(THR_NEW_LWP | THR_JOINABLE, SERVER_THREAD_POOL_SIZE); // Server::svc called()
    ACE_Thread_Manager::instance()->wait(); // wait all threads join

    return 0;
}

