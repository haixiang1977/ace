// receive message from socket then put into queue and another handler process packet from queue

#include <stdio.h>

#include "ace/Task_T.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Log_Msg.h"

ACE_Message_Queue<ACE_MT_SYNCH> q(2 * 1024 * 1024);  // 2MB high water

class ProcessHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> {
 public:
    // override the ACE_Event_Handler::handle_input()
    virtual int handle_input(ACE_HANDLE fd);
};

int ProcessHandler::handle_input(ACE_HANDLE fd) {
    printf("ProcessHandler::handle_input() called\n");

    char buf[4096];
    size_t bytes_received;

    while ((bytes_received = this->peer_.recv(buf, sizeof(buf) - 1)) > 0) {
        buf[bytes_received] = 0;

        ACE_Message_Block *mb;
        mb = new ACE_Message_Block((const char*)buf, bytes_received);
        q.enqueue(mb);
    }

            
    if (bytes_received > 0) {
        return 0;
    } else {
        return -1;
    }
}

class CommandHandler : public ACE_Task<ACE_MT_SYNCH> {
 public:
    // Run by a daemon thread to handle deferred processing.
    virtual int svc(void) {
        printf("Handler Thread running\n");

        while (1) {
            ACE_Message_Block *mb;
            q.dequeue(mb);

            printf("message %s\n", mb->rd_ptr());

            mb->release();
        }

        return 0;
    }
};

int main(int argc, char* argv[]) {
    // start the task
    CommandHandler handler;
    handler.activate(THR_NEW_LWP | THR_JOINABLE);

    ACE_Acceptor<ProcessHandler, ACE_SOCK_ACCEPTOR>* acceptor = new ACE_Acceptor<ProcessHandler, ACE_SOCK_ACCEPTOR>;

    ACE_INET_Addr port_to_listen(50000);
    if (acceptor->open(port_to_listen) == -1) {
        printf("open port failure\n");
        return -1;
    }

    if (ACE_Reactor::instance()->register_handler(acceptor, ACE_Event_Handler::READ_MASK) == -1) {
        printf("register accept handler failure\n");
        return -1;
    }

    // run reactor event loop
    ACE_Reactor::instance()->run_reactor_event_loop();

    delete acceptor;

    return 0;
}
