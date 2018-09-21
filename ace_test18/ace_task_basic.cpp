#include <stdio.h>
#include <unistd.h>

#include "ace/Task.h"

class CommandHandler : public ACE_Task_Base {
 public:
    // Run by a daemon thread to handle deferred processing.
    virtual int svc(void) {
        printf("Handler Thread running\n");

        printf("Handler Thread quit\n");
        return 0;
    }
};

int main(int argc, char* argv[]) {
    printf("main thread running\n");

    CommandHandler handler;

    int ret = handler.activate(THR_NEW_LWP | THR_JOINABLE | THR_SUSPENDED);

    printf("the current thread count is %d", handler.thr_count());

    sleep (5);

    // start handler from suspend
    handler.resume();

    // wait handler finish
    handler.wait();

    return 0;
}


