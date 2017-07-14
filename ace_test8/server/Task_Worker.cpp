#include "ace/OS.h"
#include "ace/Message_Block.h"

#include "Task_Worker.h"

namespace ACE_Server
{

int Task_Worker::svc(void)
{
    while(1) {
        ACE_Message_Block *mb = NULL;

        if (this->getq(mb) == -1) {
            continue;
        } else {
            process_task(mb);
        }
    }
    return 0;
}


void Task_Worker::process_task(ACE_Message_Block *mb)
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("Process Task: %s length %d"), mb->rd_ptr(), mb->length()));
    mb->release();

    return;
}

}

