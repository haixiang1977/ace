#include <stdio.h>

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Log_Msg.h"

// one of the common case of the reactor framework is to handle network I/O
// a simple server scenario requests 2 event handlers:
// 1. process incoming connection requests (accept)
// 2. process a client connection (receive data request and send response)

// class to process incoming data request
class ProcessHandler : public ACE_Event_Handler {
 private:
    // stream socket for data exchange
    ACE_SOCK_Stream mStream;

 public:
    // constructor
    ProcessHandler();

    // destructor
    virtual ~ProcessHandler();

    // provides access to internal stream socket
    ACE_SOCK_Stream& getSockStream();

    // begin: override ACE_Event_Handler function
    virtual ACE_HANDLE get_handle(void) const;

    // called when data exchange between client and server
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);

    // called when a <handle_*()> methods return -1 or when the <remove_handler> method is called on an ACE_Reactor
    virtual int handle_close(ACE_HANDLE fd, ACE_Reactor_Mask close_mask);
    // end: override ACE_Event_Handler function
};

ProcessHandler::ProcessHandler() {
    printf("ProcessHandler::ProcessHandler() called\n");
}

ProcessHandler::~ProcessHandler() {
    printf("ProcessHandler::~ProcessHandler() called\n");
}

ACE_SOCK_Stream& ProcessHandler::getSockStream() {
    printf("ProcessHandler::getSockStream() called\n");

    return mStream;
}

ACE_HANDLE ProcessHandler::get_handle() const {
    printf("ProcessHandler::get_handle() called\n");

    return mStream.get_handle();
}

// called when data available in socket
/*
 (gdb) bt
 #0  ProcessHandler::handle_input (this=0x965aea8, fd=9)
    at ace_reactor_server.cpp:65
 #1  0xb777af8e in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::notify_handle ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #2  0xb777802b in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_set ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #3  0xb777439f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_handlers ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #4  0xb7775788 in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #5  0xb777929f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::handle_events ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #6  0xb77d6913 in ACE_Reactor::run_reactor_event_loop ()
---Type <return> to continue, or q <return> to quit---
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #7  0x08049972 in main () at ace_reactor_server.cpp:254
 (gdb)
 */
int ProcessHandler::handle_input(ACE_HANDLE fd) {
    printf("ProcessHandler::handle_input() called\n");

    char buf[4096];
    size_t bytes_received;

    while ((bytes_received = mStream.recv(buf, sizeof(buf) - 1)) > 0) {
            buf[bytes_received] = 0;
            // send the response
            mStream.send_n (buf, bytes_received + 1);
    }

    if (bytes_received > 0) {
        return 0;
    } else {
        return -1;
    }
}

int ProcessHandler::handle_close(ACE_HANDLE fd, ACE_Reactor_Mask close_mask) {
    printf("ProcessHandler::handle_close() called\n");
    delete this;

    return 0;
}

// class to process incoming connections
class AcceptHandler : public ACE_Event_Handler {
 private:
    // the socket used for incoming connections
    ACE_SOCK_Acceptor mAcceptor;

 public:
    // constructor
    AcceptHandler();

    // destructor
    virtual ~AcceptHandler();

    // open the listening the socket and register the handler with the reactor
    int open();

    // begin: override ACE_Event_Handler function
    // Get the I/O handle (Gets the file descriptor associated with this I/O device)
    virtual ACE_HANDLE get_handle(void) const;

    // called when input events occured (e.g. connection or data)
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);

    // called when a <handle_*()> methods return -1 or when the <remove_handler> method is called on an ACE_Reactor
    virtual int handle_close(ACE_HANDLE fd, ACE_Reactor_Mask close_mask);
    // end: override ACE_Event_Handler function
};

AcceptHandler::AcceptHandler() {
    printf("AcceptHandler::AcceptHandler() called\n");
}

AcceptHandler::~AcceptHandler() {
    printf("AcceptHandler::~AcceptHandler() called\n");
}

int AcceptHandler::open() {
    printf("AcceptHandler::open() called\n");

    ACE_INET_Addr port_to_listen(50000);

    // open a port
    if (mAcceptor.open(port_to_listen, 1) == -1) {
        printf("open port failure\n");
        return -1;
    }

    // register the handler with reactor
    if (ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK) == -1) {
        printf("register accept handler failure\n");

        if (mAcceptor.close() == -1) {
            printf("close port failure\n");
        }
        return -1;
    }

    return 0;
}

/*
 (gdb) bt
 #0  AcceptHandler::get_handle (this=0x97934a8) at ace_reactor_server.cpp:76
 #1  0xb77d8edf in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::register_handler ()
    from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #2  0xb7836b9b in ACE_Reactor::register_handler ()
    from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #3  0x0804921b in AcceptHandler::open (this=0x97934a8)
     at ace_reactor_server.cpp:64
 #4  0x08049591 in main () at ace_reactor_server.cpp:97
 (gdb)
 */
ACE_HANDLE AcceptHandler::get_handle(void) const {
    printf("AcceptHandler::get_handle() called\n");

    /*
     (gdb) bt
     #0  ACE_IPC_SAP::get_handle (this=0x8dd74c0)
        at /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/include/ace/IPC_SAP.inl:19
     #1  0x08049168 in AcceptHandler::get_handle (this=0x8dd74a8)
        at ace_reactor_server.cpp:91
     #2  0xb7725edf in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::register_handler ()
        from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
     #3  0xb7783b9b in ACE_Reactor::register_handler ()
        from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
     #4  0x08049223 in AcceptHandler::open (this=0x8dd74a8)
        at ace_reactor_server.cpp:64
     #5  0x08049599 in main () at ace_reactor_server.cpp:109
     (gdb)

     ACE_INLINE ACE_HANDLE
     ACE_IPC_SAP::get_handle (void) const
     {
        ACE_TRACE ("ACE_IPC_SAP::get_handle");
        return this->handle_;
     }
     */
    return mAcceptor.get_handle();
}

// called when client connect to server
/*
 (gdb) bt
 #0  AcceptHandler::handle_input (this=0x88cf4a8, fd=6)
    at ace_reactor_server.cpp:117
 #1  0xb77a2f8e in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::notify_handle ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #2  0xb77a002b in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_set ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #3  0xb779c39f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch_io_handlers ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #4  0xb779d788 in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::dispatch ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #5  0xb77a129f in ACE_Select_Reactor_T<ACE_Reactor_Token_T<ACE_Token> >::handle_events ()
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #6  0xb77fe913 in ACE_Reactor::run_reactor_event_loop ()
 ---Type <return> to continue, or q <return> to quit---
   from /home/developer/fca/gateway/src/openembedded/tmp/staging/i686-linux/usr/lib/libACE-5.6.7.so
 #7  0x0804963a in main () at ace_reactor_server.cpp:143
 (gdb)
 */
int AcceptHandler::handle_input(ACE_HANDLE fd) {
    printf("AcceptHandler::handle_input() called\n");

#if 1
    // create a new data process handler
    ProcessHandler* processHandler = new ProcessHandler();

    // accept the connect request
    if (mAcceptor.accept(processHandler->getSockStream()) == -1) {
        printf("accept connection failure\n");
        return -1;
    }

    // register the data process handler with reactor
    if (ACE_Reactor::instance()->register_handler(processHandler, ACE_Event_Handler::READ_MASK) == -1) {
        printf("register data process handler failure\n");
        return -1;
    }

#else // simple test

    ACE_SOCK_Stream peer;

    if (mAcceptor.accept(peer) == -1) {
        printf("accept connection failure\n");
        return -1;
    } else {
        printf("accept connection OK\n");
    }
#endif

    return 0;
}

int AcceptHandler::handle_close(ACE_HANDLE fd, ACE_Reactor_Mask close_mask) {
    printf("AcceptHandler::handle_close() called\n");

    // close the listening port
    if (mAcceptor.close() == -1) {
        printf("close port failure\n");
    }
    delete this;

    return 0;
}

int main(int argc, char* argv[]) {
    AcceptHandler* acceptHandler = new AcceptHandler();

    if (acceptHandler->open() == -1) {
        delete acceptHandler;
        return -1;
    }

    // run reactor event loop
    ACE_Reactor::instance()->run_reactor_event_loop();
}
