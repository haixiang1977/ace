#include <stdio.h>
#include <unistd.h>

#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Future.h"
#include "ace/Method_Request.h"
#include "ace/Activation_Queue.h"

#define USE_FUTURE_OBSERVER 1

// ACE_Future
// This class implements a ``single write, multiple read'' pattern that can be used to return results from asynchronous method invocations.

// real implementation
class HA_Controller {
 public:
    HA_Controller();

    int status_update();
 private:
    int status_result_;
};

HA_Controller::HA_Controller() {
    printf("HA_Controller::HA_Controller() called\n");

    status_result_ = 0;
}

int HA_Controller::status_update() {
    printf("HA_Controller::status_update() called\n");

    // simluate send command to controller and get status back
    sleep(2);

    return ++status_result_;
}

// request method
class StatusUpdateRequestMethod : public ACE_Method_Request {
 public:
    StatusUpdateRequestMethod(HA_Controller& controller, ACE_Future<int>& ret);

    virtual int call();
 private:
    HA_Controller& controller_;
    ACE_Future<int> returnVal_;
};

StatusUpdateRequestMethod::StatusUpdateRequestMethod(HA_Controller& controller, ACE_Future<int>& ret) : \
    controller_(controller), returnVal_(ret) {
    printf("StatusUpdateRequestMethod::StatusUpdateRequestMethod() called\n");
}

int StatusUpdateRequestMethod::call() {
    printf("StatusUpdateRequestMethod::called() called\n");

    // update the latest value from real implementation
    this->returnVal_.set(this->controller_.status_update());

    return 0;
}

class ExitRequestMethod : public ACE_Method_Request {
 public:
    virtual int call();
 private:
};

int ExitRequestMethod::call() {
    printf("ExitRequestMethod::called() called\n");

    return -1;
}

// scheduler class
class Method_Scheduler : public ACE_Task_Base {
 public:
    Method_Scheduler();

    virtual int svc();

    int enqueue(ACE_Method_Request* rqt);
 private:
    ACE_Activation_Queue activation_queue_;
};

Method_Scheduler::Method_Scheduler() {
    printf("Method_Scheduler::Method_Scheduler() called\n");

    // activate the task
    this->activate();
}

int Method_Scheduler::svc() {
    printf("Method_Scheduler::svc() called\n");

    while(1) {
        // dequeue the next method request
        auto_ptr<ACE_Method_Request> rqt(this->activation_queue_.dequeue());

        if (rqt->call() == -1) {
            break;
        }
    }

    return 0;
}

int Method_Scheduler::enqueue(ACE_Method_Request* rqt) {
    printf("Method_Scheduler::enqueue() called\n");

    return this->activation_queue_.enqueue(rqt);
}

// proxy implemenation and hide the detail of real implementation
class HA_ControllerProxy {
 public:
    ACE_Future<int> status_update();

    void exit();
 private:
    Method_Scheduler scheduler_;
    // real controller implementation
    HA_Controller controller_;
};

ACE_Future<int> HA_ControllerProxy::status_update() {
    printf("HA_ControllerProxy::status_update() called\n");
    ACE_Future<int> ret;

    // new a request method then push into queue
    this->scheduler_.enqueue(new StatusUpdateRequestMethod(this->controller_, ret));

    // application call get() to wait for result return
    // here has no memory free problem when exit this function, because ACE_Future constructor create a pointer
    return ret;
}

void HA_ControllerProxy::exit() {
    printf("HA_ControllerProxy::exit() called\n");

    this->scheduler_.enqueue(new ExitRequestMethod());

    return;
}

#ifdef USE_FUTURE_OBSERVER
class CompletionCallback : public ACE_Future_Observer<int> {
 public:
    CompletionCallback(HA_ControllerProxy& proxy);
    virtual void update(const ACE_Future<int>& future);
 private:
    HA_ControllerProxy& proxy_;
};

CompletionCallback::CompletionCallback(HA_ControllerProxy& proxy) : proxy_(proxy) {
    printf("CompletionCallback::CompletionCallback() called\n");
}

void CompletionCallback::update(const ACE_Future<int>& future) {
    printf("CompletionCallback::update() called\n");

    int result;
    future.get(result);
    printf("new status update %d\n", result);

    if (result == 10) {
        this->proxy_.exit();
    }

    return;
}

#endif

int main(int argc, char* argv[]) {
    HA_ControllerProxy proxy;
    ACE_Future<int> results[10];
#ifdef USE_FUTURE_OBSERVER
    CompletionCallback cb(proxy);
#endif

    for (int i = 0; i < 10; i++) {
        printf("proxy status update %d\n", i);
        results[i] = proxy.status_update();
#ifdef USE_FUTURE_OBSERVER
        results[i].attach(&cb);
#endif
    }

#ifdef USE_FUTURE_OBSERVER
#else
    sleep (3);

    for (int j = 0; j < 10; j++) {
        int ret;
        if (results[j].get(ret) == 0) {
            printf("new status [%d] update %d\n", j, ret);
        }
    }

    proxy.exit();
#endif

    ACE_Thread_Manager::instance()->wait();

    return 0;
}
