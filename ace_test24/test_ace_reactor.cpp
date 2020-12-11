# to reproduce the crash notify after the reactor close
# build command:
# g++ -g -O0 test_ace_reactor.cpp -o test_ace_reactor.out -lACE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>

#include "ace/Reactor.h"
#include "ace/Task.h"
#include "ace/Synch.h"

class MasterSession : public ACE_Event_Handler {
 public:
    MasterSession() {};
    virtual ~MasterSession() {};

    virtual int handle_input(ACE_HANDLE);

    // notify the reactor
    int send();

 private:
    ;
};

int MasterSession::handle_input(ACE_HANDLE)
{
    printf("MasterSession::handle_input called\n");

    return 0;
}

int MasterSession::send()
{
    reactor()->notify(this, ACE_Event_Handler::READ_MASK);
    return 0;
}

class ReactorProxy {
 public:
    ReactorProxy() : reactor(), alive(true) {};
    virtual ~ReactorProxy() {};

    virtual int run_reactor_event_loop();
    virtual int end_reactor_event_loop();

    ACE_Reactor* get() {return &reactor;}

 private:
    ACE_Reactor reactor;
    bool alive;
};

int ReactorProxy::run_reactor_event_loop() {
    reactor.owner(ACE_Thread::self());

    printf("reactor loop running\n");
    while (alive) {
        int status = reactor.handle_events();
        if (status == -1) {
            printf("had reactor error\n");
            alive = false;
        }
    }

    reactor.close();

    printf("reactor loop quitted\n");

    return 0;
}

int ReactorProxy::end_reactor_event_loop() {
    alive = false;
    printf("end reactor loop\n");

    return 0;
}

class SendTask : public ACE_Task<ACE_NULL_SYNCH> {
 public:
    SendTask(MasterSession* m, ReactorProxy* p) : m_master(m), m_proxy(p) {};
    virtual ~SendTask() {};

    virtual int svc();

 private:
    MasterSession* m_master;
    ReactorProxy* m_proxy;
};

int SendTask::svc() {
    int count = 0;
    printf("SendTask::svc started\n");

    while (1) {
        if (count == 30) {
            // trig the crash
            m_proxy->end_reactor_event_loop();
        }
        sleep(2);
        m_master->send();

        count ++;
    }

    return 0;
}

int main() {
    MasterSession master;
    ReactorProxy proxy;
    SendTask send_task(&master, &proxy);

    // binding reactor with master
    master.reactor(proxy.get());

    // run task
    send_task.activate();
    
    // run reactor
    proxy.run_reactor_event_loop();

    // wait for reactor close
    // endless loop here
    while(1) {
        sleep(1);
    }

    return 0;
}
