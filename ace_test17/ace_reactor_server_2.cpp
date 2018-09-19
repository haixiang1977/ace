// simplify the server code to use ACE_Svc_Handler and ACE_Acceptor

#include <stdio.h>

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Log_Msg.h"

class ProcessHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> {
 public:
    // constructor
    ProcessHandler();

    // deconstructor
    virtual ~ProcessHandler();

    // override the ACE_Event_Handler::handle_input()
    // http://www.dre.vanderbilt.edu/Doxygen/5.6/html/ace/classACE__Svc__Handler.html
    virtual int handle_input(ACE_HANDLE fd);

    // all others are hidden in ACE_Svc_Handler class
};

/*
(gdb) bt
 #0  ProcessHandler (this=0x8ec2f30) at ace_reactor_server_2.cpp:29
 #1  0x0804f8bf in ACE_Acceptor<ProcessHandler, ACE_SOCK_Acceptor>::make_svc_handler (this=0x8ec04a8, sh=@0xbfe4bec0) at /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/include/ace/Acceptor.cpp:265
 #2  0x0804e265 in ACE_Acceptor<ProcessHandler, ACE_SOCK_Acceptor>::handle_input (this=0x8ec04a8, listener=8) at /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/include/ace/Acceptor.cpp:392
 #3  0xb7737f8e in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::notify_handle () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #4  0xb773502b in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_set () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #5  0xb773139f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_handlers () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #6  0xb7732788 in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #7  0xb773629f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::handle_events () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #8  0xb7793913 in ACE_Reactor::run_reactor_event_loop () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #9  0x0804bb63 in main (argc=0, argv=0xbfe4c154) at ace_reactor_server_2.cpp:71
(gdb)
 */
ProcessHandler::ProcessHandler() {
    printf("ProcessHandler::ProcessHandler() called\n");
}

/*
(gdb) bt
#0  ~ProcessHandler (this=0x8d68f30) at ace_reactor_server_2.cpp:33
#1  0x0804cef1 in ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>::destroy (this=0x8d68f30) at /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/include/ace/Svc_Handler.cpp:118
#2  0x0804cca8 in ACE_Svc_Handler<ACE_SOCK_Stream, ACE_NULL_SYNCH>::handle_close (this=0x8d68f30) at /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/include/ace/Svc_Handler.cpp:305
#3  0xb7830b54 in ACE_Select_Reactor_Handler_Repository::unbind () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#4  0xb77c292b in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::remove_handler_i () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#5  0xb77c806f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::notify_handle () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#6  0xb77c502b in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_set () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#7  0xb77c139f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_handlers () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#8  0xb77c2788 in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#9  0xb77c629f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::handle_events () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#10 0xb7823913 in ACE_Reactor::run_reactor_event_loop () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#11 0x0804bb63 in main (argc=134544168, argv=0xbfef5304) at ace_reactor_server_2.cpp:71
(gdb)
 */
ProcessHandler::~ProcessHandler() {
    printf("ProcessHandler::~ProcessHandler() called\n");
}

/*
(gdb) bt
#0  ProcessHandler::handle_input (this=0x8470f30, fd=9) at ace_reactor_server_2.cpp:37
#1  0xb76fbf8e in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::notify_handle () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#2  0xb76f902b in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_set () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#3  0xb76f539f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_handlers () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#4  0xb76f6788 in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#5  0xb76fa29f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::handle_events () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#6  0xb7757913 in ACE_Reactor::run_reactor_event_loop () from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
#7  0x0804bb63 in main (argc=Cannot access memory at address 0x9
) at ace_reactor_server_2.cpp:71
(gdb)
 */
int ProcessHandler::handle_input(ACE_HANDLE fd) {
    printf("ProcessHandler::handle_input() called\n");

    char buf[4096];
    size_t bytes_received;

    while ((bytes_received = this->peer_.recv(buf, sizeof(buf) - 1)) > 0) {
            buf[bytes_received] = 0;
            // send the response
            this->peer_.send_n (buf, bytes_received + 1);
    }

    if (bytes_received > 0) {
        return 0;
    } else {
        return -1;
    }
}

int main(int argc, char* argv[]) {
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
