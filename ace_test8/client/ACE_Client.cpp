#include "Client.h"

using namespace ACE_Client;

int main(int argc, char *argv[])
{
    Client client("localhost");

    for (int i = 0; i < 5; i++) {
        char *task1 = "is it a good day?";
        size_t task1_len = 18;
        char *task1_t;

        ACE_NEW_RETURN(task1_t, char[task1_len + 4], -1); // ACE_NEW_RETURN() is ACE new() wrapper, if new() failed, then caller return -1
                                                         // add extra 4 bytes to put the message length information inside
        client.put_task(task1_t, task1, task1_len);

        char *task2 = "Yeah, it really is.";
        size_t task2_len = 20;
        char *task2_t;
        ACE_NEW_RETURN(task2_t, char[task2_len + 4], -1);
        client.put_task(task2_t, task2, task2_len);

        client.send_tasks();

        delete [] task1_t;
        delete [] task2_t;
    }

    return 0;
}

