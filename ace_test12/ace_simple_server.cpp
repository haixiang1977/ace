// ace sample server
#include <stdio.h>

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"

int main (int argc, char* argv[]) {
    ACE_INET_Addr port_to_listen(50000);
    ACE_SOCK_Acceptor acceptor;

    // open port and listen
    if (acceptor.open(port_to_listen) == -1) {
        printf("open port failure\n");
        return -1;
    }

    // start to listen
    while (1) {
        ACE_SOCK_Stream peer;

        // accept the coming connection request
        if (acceptor.accept(peer) == -1) {
            printf("accept failure\n");
            return -1;
        }

        char buf[4096];
        size_t bytes_received;

        // receive the request
        while ((bytes_received = peer.recv(buf, sizeof(buf) - 1)) > 0) {
            buf[bytes_received] = 0;
            // send the response
            peer.send_n (buf, bytes_received + 1);
        }

        // close the connection
        peer.close();
    }

    return 0;
}
