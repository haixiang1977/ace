#ifndef __TASK_WORKER_H_
#define __TASK_WORKER_H_

#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Message_Block.h"

namespace ACE_Server
{

class Task_Worker : public ACE_Task <ACE_MT_SYNCH>
{
    public:
        virtual int svc(void);
    private:
        void process_task(ACE_Message_Block *mb);
};

}

#endif

