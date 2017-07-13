#ifndef __CLIENT_H_
#define __CLIENT_H_

#include "ace/Task.h"
#include "ace/INET_Addr.h"
#include "ace/Synch.h"

namespace ACE_Client {
    class Client : public ACE_Task<ACE_NULL_SYNCH> { // ACE_NULL_SYNC: no sync wrapper, don't use mutex and condition variable
        public:
            Client(char* server);

            virtual int svc(void);

            char* put_task(char* msg_t, char* msg_s, size_t msg_len);

            void send_tasks(void);
        private:
            ACE_INET_Addr addr;
    };
}

#endif

