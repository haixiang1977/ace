#include "ace/OS.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Message_Block.h"
#include "ace/Thread_Manager.h"
#include "ace/INET_Addr.h"

#include "Constants.h"
#include "Client.h"

namespace ACE_Client
{

Client::Client(char* server)
{
    addr = ACE_INET_Addr(SERVER_PORT_NUM, server);
}

int Client::svc(void)
{
    ACE_SOCK_Stream stream;
    ACE_SOCK_Connector connector;

    if (connector.connect(stream, addr) < 0) {
        return -1;
    }

    while(1) {
        ACE_Message_Block *mb = NULL;

        if (this->getq(mb) == -1) { // retrieve message from queue
            break;
        }

        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t)Sending %s\n"), &(mb->rd_ptr()[4])));

        stream.send_n(mb->rd_ptr(), mb->length());
        mb->release();

        ACE_OS::sleep(TIME_INTERVAL);
    }

    stream.close();

    return 0;
}

void Client::send_tasks(void)
{
    this->activate();
    ACE_Thread_Manager::instance()->wait();
    return;
}

char* Client::put_task(char* msg_t, char* msg_s, size_t msg_len)
{
    for (int i = 0; i < 4; i++) { // put the message length information into header 4 bytes
        msg_t[i] = (char)((msg_len >> (8 * i)) & 0xFF);
    }
    ACE_OS::memcpy(&msg_t[4], msg_s, msg_len);

    ACE_Message_Block *mb = NULL;
    ACE_NEW_RETURN(mb, ACE_Message_Block(msg_len + 4, ACE_Message_Block::MB_DATA, 0, msg_t), 0); // create an initilized message of type containing size bytes
    mb->wr_ptr(msg_len + 4);
    this->putq(mb);

    return msg_t;
}

}

