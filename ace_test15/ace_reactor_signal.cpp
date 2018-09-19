#include <stdio.h>

#include <ace/Reactor.h>
#include <ace/Event_Handler.h>

class MySignalHandler : public ACE_Event_Handler {
 public:
    MySignalHandler() : ACE_Event_Handler() {
        printf("MySignalHandler::MySignalHandler() called\n");
        count = 0;
    }

    ~MySignalHandler() {
        printf("MySignalHandler::~MySignalHandler() called\n");
    }

    int handle_signal (int signum, siginfo_t* = 0, ucontext_t* = 0) {
        printf("MySignalHandler::handle_signal() called\n");
        count ++;

        if (count == 3) {
            return -1;
        } else {
            return 0;
        }
    }

    int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask) {
        printf("MySignalHandler::handle_close() called\n");

        delete this;

        return 0;
    }

 private:
    int count;
};

int main(int argc, char* argv[]) {
    MySignalHandler *handler = new MySignalHandler();

    // register ctrl+c signal
    ACE_Reactor::instance()->register_handler(SIGINT, handler);

    ACE_Reactor::instance()->run_reactor_event_loop();

    return 0;
}
