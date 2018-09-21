#include <stdio.h>
#include <unistd.h>

#include "ace/Task.h"

class CancelTask : public ACE_Task<ACE_MT_SYNCH> {
 public:
    virtual int svc() {
        printf("CancelTask startup\n");

        while (1) {
            if (thr_mgr()->testcancel(thr_mgr()->thr_self()) == true) {
                printf("CancelTask exit\n");
                break;
            }

            sleep(1);
        }

        return 0;
    }
};

int main(int argc, char* argv[]) {
    CancelTask cancel_task;

    cancel_task.activate();

    sleep(3);

    printf("CancelTask cancelled\n");
    ACE_Thread_Manager::instance()->cancel_task(&cancel_task);

    cancel_task.wait();

    return 0;
}
