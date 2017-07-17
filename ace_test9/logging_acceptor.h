#ifndef __LOGGING_ACCEPTOR_H_
#define __LOGGING_ACCEPTOR_H_

#include "ace/SOCK_Acceptor.h"
#include "ace/Event_Handler.h"

#include "logging_handler.h"

class Logging_Acceptor : public ACE_Event_Handler
{
public:
    int open(const ACE_INET_Addr &addr, ACE_Reactor *reactor) {
        // open the acceptor
        if (this->peer_acceptor_.open(addr, 1) == -1) {
            ACE_ERROR_RETURN((LM_ERROR, "Error Opening Logging_Acceptor\n"), -1);
        }

        reactor_ = reactor;

        // regist the reactor
        if (reactor->register_handler(this, ACE_Event_Handler::ACCEPT_MASK) == -1) {
            ACE_ERROR_RETURN((LM_ERROR, "Error reactor register_handler\n"), -1);
        }
    }

private:
    ACE_HANDLE get_handle(void) const {
        return this->peer_acceptor_.get_handle();
    }

    // When an accept request arrives, the reactor will invoke the handle_input() callback.
    // This is where we deal with the connection request.
    virtual int handle_input(ACE_HANDLE handle) {
        // here we don't use it
        ACE_UNUSED_ARG (handle);

        Logging_Handler *svc_handler;

        ACE_NEW_RETURN(svc_handler, Logging_Handler, -1);

        // To complete the connection, we invoke the accept() method call on the acceptor object and provide it
        // with the connection handler instance.
        if (this->peer_acceptor_.accept(*svc_handler) == -1) {
            ACE_ERROR_RETURN ((LM_ERROR, "%p", "accept failed"), -1);
        }

        if (svc_handler->open(reactor_) == -1) {
            svc_handler->close();
        }

        return 0;
    }

protected:
    ACE_SOCK_Acceptor peer_acceptor_;

    ACE_Reactor *reactor_;
};

#endif
