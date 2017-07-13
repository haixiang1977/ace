#ifndef __SERVER_H_
#define __SERVER_H_

#include "ace/Task.h"

namespace ACE_Server
{
    class Server : public ACE_Task_Base
    {
        public:
            virtual int svc(void);
    };
}

#endif

