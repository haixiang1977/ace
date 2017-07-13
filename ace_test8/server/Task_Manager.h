#ifndef __TASK_MANAGER_H_
#define __TASK_MANAGER_H

#include "ace/Task.h"
#include "ace/Synch.h"

namespace ACE_Server
{

class Task_Manager : public ACE_Task<ACE_MT_SYNCH>
{
    public:
        virtual int svc(void);
};

}

#endif

