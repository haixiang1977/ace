// ace sample client
#include <stdio.h>
#include <unistd.h>

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Log_Msg.h"

int main(int argc, char *argv[]) {
    // set server ip:port
    ACE_INET_Addr srvr (50000, ACE_LOCALHOST); // port 50000, ip local host

    ACE_SOCK_Connector connector;
    ACE_SOCK_Stream peer;

    // connect to the server
    printf("connecting ...\n");
    if (-1 == connector.connect(peer, srvr)) {
        printf("connect failure\n");
        return -1;
    }

    int bc;
    char buf[64];

    // send request
    peer.send_n("uptime\n", 7);

    // receive response
    bc = peer.recv(buf, sizeof(buf));

    // print to console
    write(1, buf, bc);

    // sleep 5 minutes then close the connection
    sleep(5);

    // close the connection
    peer.close();

    return 0;
}
