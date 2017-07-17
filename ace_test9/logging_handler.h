#ifndef __LOGGING_HANDLER_H_
#define __LOGGING_HANDLER_H_

#include "ace/Log_Msg.h"
#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h" // use TCP

class Logging_Handler : public ACE_Event_Handler
{
public:
    int open(ACE_Reactor *reactor) {
        reactor_ = reactor;

        if (reactor->register_handler(this, ACE_Event_Handler::READ_MASK) == -1) {
            ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) can't register with reactor"), -1);
        }
        return 0;
    }

    int close(void) {
        return this->handle_close(ACE_INVALID_HANDLE, ACE_Event_Handler::RWE_MASK);
    }

    operator ACE_SOCK_Stream &() {
        return this->cli_stream_;
    }

protected:
    ACE_HANDLE get_handle(void) const {
        return this->cli_stream_.get_handle();
    }

    virtual int handle_input(ACE_HANDLE) {
        char buf[BUFSIZ + 1];

        ssize_t retval;

        switch(retval = this->cli_stream_.recv(buf, BUFSIZ))
        {
            case -1:
                ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p bad read\n", "client logger"), -1);
            case 0:
                ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) closing log daemon (fd = %d)\n", this->get_handle ()), -1);
            default:
                buf[retval] = '\0';
                ACE_DEBUG ((LM_DEBUG, "(%P|%t) from client: %s", buf));
                break;
        }
        return 0;
    }

    int handle_close(ACE_HANDLE, ACE_Reactor_Mask _mask) {
        reactor_->remove_handler(this, _mask | ACE_Event_Handler::DONT_CALL);

        cli_stream_.close();

        delete this;

        return 0;
    }

private:
    ACE_SOCK_Stream cli_stream_;

    ACE_Reactor *reactor_;
};

#endif
