#include <stdio.h>
#include <unistd.h>

#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Synch.h"

class Worker;

class IManager {
 public:
    virtual ~IManager() {}

    virtual int return_to_work(Worker* worker) = 0;
};

class Manager : public ACE_Task<ACE_MT_SYNCH>, IManager {
 public:
    enum {POOL_SIZE = 5, MAX_TIMEOUT = 5};

    Manager();

    virtual int svc();

    virtual int return_to_work(Worker* worker);
 private:
    int create_worker_pool();

    int shut_down_;
    ACE_Thread_Mutex workers_lock_;
    ACE_Condition<ACE_Thread_Mutex> workers_cond_;
    // Unbounded queue means queue with infinity length
    ACE_Unbounded_Queue<Worker* > workers_;
};

class Worker : public ACE_Task<ACE_MT_SYNCH> {
 public:
    Worker(IManager* manager);

    virtual int svc();

    unsigned int get_thread_id();
 private:
    IManager* manager_;
    unsigned int thread_id_;
};

Manager::Manager() : shut_down_(0), workers_lock_(), workers_cond_(workers_lock_) {
    printf("Manager::Manager() called\n");
}

int Manager::svc() {
    printf("Manager::svc() called\n");

    create_worker_pool();

    while(1) {
        ACE_Message_Block *mb = NULL;

        // get incoming message
        if (getq(mb) < 0) {
            printf("Manager::svc() failed to get message from queue\n");
            return 0;
        }

        // choose a worker
        Worker* worker;
        {
            ACE_GUARD_RETURN(ACE_Thread_Mutex, workers_mon, workers_lock_, -1);

            while (workers_.is_empty()) {
                // all workser are busy
                workers_cond_.wait();
            }

            // now we have free worker here
            workers_.dequeue_head(worker);
        }

        // put message into worker for handling
        worker->putq(mb);
    }

    return 0;
}

int Manager::return_to_work(Worker* worker) {
    printf("Manager::return_to_work() worker %u called\n", worker->get_thread_id());

    ACE_GUARD_RETURN(ACE_Thread_Mutex, workers_mon, workers_lock_, -1);

    // put the free worker into pool
    workers_.enqueue_tail(worker);

    // signal
    workers_cond_.signal();

    return 0;
}

int Manager::create_worker_pool() {
    printf("Manager::create_worker_pool() called\n");

    ACE_GUARD_RETURN(ACE_Thread_Mutex, workers_mon, workers_lock_, -1);

    for (int i = 0; i < POOL_SIZE; i++) {
        Worker* worker = new Worker(this);

        // push workers into pool
        workers_.enqueue_tail(worker);
        worker->activate();
    }

    return 0;
}

Worker::Worker(IManager* manager) : manager_(manager) {
    printf("Worker::Worker() called\n");
}

int Worker::svc() {
    thread_id_ = (int)ACE_Thread::self ();
    while (1) {
        ACE_Message_Block *mb = NULL;

        if (getq(mb) < 0) {
            printf("Worker::svc() failed to get message from queue\n");
            return 0;
        }

        // process the message
        int id;
        ACE_OS::memcpy (&id, mb->rd_ptr (), sizeof(int));
        printf("message id %d from worker %u\n", id, thread_id_);

        mb->release();

        manager_->return_to_work(this);
    }

    return 0;
}

unsigned int Worker::get_thread_id() {
    return thread_id_;
}

int main(int argc, char* argv[]) {
    Manager tp;

    // start thread pool
    tp.activate();

    sleep(1);

    // insert message to tp queue
    for (int i = 0; i < 30; i++) {
        ACE_Message_Block *mb = new ACE_Message_Block(sizeof(int));

        ACE_OS::memcpy(mb->wr_ptr(), &i, sizeof(int));

        tp.putq(mb);

        sleep(1);
    }

    return 0;
}
