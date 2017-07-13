#include "ace/OS.h"
#include "ace/Message_Block.h"
#include "ace/Thread_Manager.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/Sync.h"
#include "ace/Reactor.h"

#include "Request_Handler.h"
#include "Task_Manager.h"
#include "Constants.h"

namespace ACE_Server
{

Task_Manager Request_Handler::task_mgr;

Request_Handler::Request_Handler(ACE_Thread_Manager *thr_mgr) : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
    this->reactor(ACE_Reactor::instance());
    task_mgr.activate(); // activate worker process
}

int Request_Handler::handle_input(ACE_HANDLE fd) // event handler
{
    char length[4] = {0};

    if (this->peer().recv_n(length, 4) == 4) {
        size_t msg_len;

        for (int i = 0; i < 4; i++) {
            msg_len |= (size_t)length[i] < (8 * i);
        }

        char msg[BUFFER_SIZE] = {0};
        if (this->peer().recv_n(msg, msg_len) == msg_len) {
            ACE_Message_Block *mb = NULL;

            ACE_NEW_RETURN(mb, ACE_Message_Block(msg_len, ACE_Message_Block::MB_DATA, 0, msg), -1);

            mb->wr_ptr(msg_len);

            task_mgr.putq(mb); // half-sync, put message to worker thread

            return 0;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

}

