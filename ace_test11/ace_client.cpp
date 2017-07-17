#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"

static u_short SERVER_PORT = ACE_DEFAULT_SERVER_PORT;
static const char *const SERVER_HOST = ACE_DEFAULT_SERVER_HOST;
static const int MAX_ITERATIONS = 10;

int main(int argc, char* argv[])
{
    const char *server_host = argc > 1 ? argv[1] : SERVER_HOST;
    u_short server_port = argc > 2 ? ACE_OS::atoi (argv[2]) : SERVER_PORT;
    int max_iterations = argc > 3 ? ACE_OS::atoi (argv[3]) : MAX_ITERATIONS;

    ACE_SOCK_Stream server; // server is tcp server

    ACE_SOCK_Connector connector; // setup socket connection

    ACE_INET_Addr addr (server_port, server_host); // create socket address

    if (connector.connect (server, addr) == -1) {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "failed to open server connection"), -1);
    }

    for (int i = 0; i < max_iterations; i++) {
        char buf[BUFSIZ];

        ACE_OS::sprintf (buf, "message = %d\n", i + 1);

        if (server.send_n (buf, ACE_OS::strlen (buf)) == -1) {
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);
        }
        else {
            /* Pause for a second.  */
            ACE_OS::sleep (1);
        }
    }

    if (server.close() == -1) {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), -1);
    }

    return 0;
}

