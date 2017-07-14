#ifndef __REQUEST_HANDLER_H_
#define __REQUEST_HANDLER_H_

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/Synch.h"
#include "ace/Thread_Manager.h"

#include "Task_Manager.h"

namespace ACE_Server
{
    class Request_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
        public:
            Request_Handler(ACE_Thread_Manager *thr_mgr = 0);

        protected:
            virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);

        private:
            static Task_Manager task_mgr; // half sync
    };
}

#endif

