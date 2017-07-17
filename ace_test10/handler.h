#ifndef _LOGGING_HANDLER_H_
#define _LOGGING_HANDLER_H_

#include "ace/INET_Addr.h"
#include "ace/Reactor.h"

extern ACE_Reactor *g_reactor;

class Logging_Handler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
    virtual int open(void *) {
        ACE_INET_Addr addr;

        // ask peer client address
        if (this->peer().get_remote_addr(addr) == -1) {
            return -1;
        }

        // use the READ_MASK so that handle_input() will be called when the client does something.
        if (g_reactor->register_handler(this, ACE_Event_Handler::READ_MASK) == -1) {
            ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) can't register with reactor\n"), -1);
        }

        // schedule a timer event, first time 2 seconds, later 3 seconds
        if (g_reactor->schedule_timer(this, 0, ACE_Time_Value(2), ACE_Time_Value(3)) == -1) {
            ACE_ERROR_RETURN ((LM_ERROR, "can'(%P|%t) t register with reactor\n"), -1);
        }

        ACE_DEBUG ((LM_DEBUG, "(%P|%t) connected with %s\n", addr.get_host_name ()));

        return 0;
    }

    virtual void destroy(void) {
        g_reactor->remove_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

        g_reactor->cancel_timer(this);

        this->peer().close();

        delete this;

        return;
    }

    virtual int close(u_long flags = 0) {
        ACE_UNUSED_ARG(flags);

        this->destroy();
        return 0;
    }

protected:
    virtual int handle_input(ACE_HANDLE) {
        char buf[4096];
        ACE_OS::memset(buf, 0, sizeof(buf));

        switch(this->peer().recv(buf, sizeof(buf)))
        {
            case -1:
                ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p bad read\n", "client logger"), -1);
            case 0:
                ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) closing log daemon (fd = %d)\n", this->get_handle ()), -1);
            default:
                ACE_DEBUG ((LM_DEBUG, "(%P|%t) from client: %s", buf));
                break;
        }

        return 0;
    }

    virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg) {
        ACE_UNUSED_ARG(tv);
        ACE_UNUSED_ARG(arg);

        ACE_DEBUG ((LM_DEBUG, "(%P|%t) handling timeout from this = %u\n", this));
        return 0;
    }

    virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask) {
        this->destroy ();
        return 0;
    }
};

#endif
