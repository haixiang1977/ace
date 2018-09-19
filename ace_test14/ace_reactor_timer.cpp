#include <stdio.h>

#include <ace/Reactor.h>
#include <ace/Event_Handler.h>

class MyTimerHandler : public ACE_Event_Handler {
 public:
    MyTimerHandler() : ACE_Event_Handler() {
        printf("MyTimerHandler::MyTimerHandler() called\n");
        cnt = 0;
    }

    ~MyTimerHandler() {
        printf("MyTimerHandler::~MyTimerHandler() called\n");
    }

    int handle_timeout (const ACE_Time_Value &cur_time, const void* = 0) {
        printf("MyTimerHandler::handle_timeout() called %d\n", cnt);

        cnt ++;
        if (cnt == 5) {
            return -1; // call handle_close
        } else {
            return 0;
        }
    }

    int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask) {
        printf("MyTimerHandler::handle_close() called\n");

        delete this;

        return 0;
    }

 private:
    int cnt;
};

int main(int argc, char* argv[]) {
    MyTimerHandler *timer = new MyTimerHandler();
    ACE_Time_Value initialDelay(3);
    ACE_Time_Value interval(5);

    ACE_Reactor::instance()->schedule_timer(timer, 0, initialDelay, interval);

    ACE_Reactor::instance()->run_reactor_event_loop();

    return 0;
}
