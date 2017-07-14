#include "ace/Message_Block.h"

#include "Task_Manager.h"
#include "Task_Worker.h"
#include "Constants.h"

namespace ACE_Server
{

int Task_Manager::svc(void)
{
    Task_Worker task_tp;

    task_tp.activate(THR_NEW_LWP | THR_JOINABLE, TASK_THREAD_POOL_SIZE);

    while(1) {
        ACE_Message_Block *mb = NULL;
        if (this->getq(mb) < 0) {
            task_tp.msg_queue()->deactivate();
            task_tp.wait(); // block until there are no more threads running in this task
        } else {
            task_tp.putq(mb);
        }
    }

    return 0;
}

}

