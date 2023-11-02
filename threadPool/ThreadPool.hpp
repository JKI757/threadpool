// ThreadPoolAsync.hpp
// threadPool using std::async
//
// Created by josh on 10/26/23.

#ifndef ThreadPoolAsync_hpp
#define ThreadPoolAsync_hpp

#include <future>
#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <pthread.h>

#include "definitions.hpp"
#include "JobClass.hpp"
#include "StringJob.hpp"
#include "IntJob.hpp"


class ThreadPool {
public:
    ThreadPool();
    void Start();
    void QueueJob(jobVariantPtr job);
    void Stop();
    bool isBusy();
    void kill();

    ~ThreadPool();
    static void* ThreadLoopEntry(void* arg) {
        ThreadPool* pool = static_cast<ThreadPool*>(arg);
        pool->ThreadLoop();
        return NULL;
    }

private:
    bool should_terminate = false;
    pthread_mutex_t queueMutex;
    std::queue<jobVariantPtr> jobs;
    std::vector<std::future<void>> futures;
    std::atomic<int> activeJobs{0};  // Initialized to 0
    pthread_t mainThread;

    void ThreadLoop();
};

// Function definitions
ThreadPool::ThreadPool(){
    pthread_mutex_init(&queueMutex, NULL);
}
ThreadPool::~ThreadPool(){
    pthread_mutex_destroy(&queueMutex);

}
void ThreadPool::Start() {
    // Start the main thread loop
    pthread_create(&mainThread, NULL, ThreadPool::ThreadLoopEntry, this);
    pthread_detach(mainThread);  // Detach the thread
}

void ThreadPool::ThreadLoop() {
    while (true) {
        jobVariantPtr thisJob;
        {
            if (should_terminate) {
                std::cout << "Terminating ThreadLoop" << std::endl;
                return;
            }
            
            pthread_mutex_lock(&queueMutex);
            bool jobsAvail = !jobs.empty();
            pthread_mutex_unlock(&queueMutex);
            if (jobsAvail) {
                pthread_mutex_lock(&queueMutex);
                thisJob = jobs.front();
                jobs.pop();
                pthread_mutex_unlock(&queueMutex);
//                std::cout << "Popped a job from the queue" << std::endl;
            } else {
//                std::cout << "No jobs in the queue" << std::endl;
                continue;
            }
            
        }
        ++activeJobs;
        futures.push_back(std::async(std::launch::async, [this, thisJob] {
//            std::cout << "Processing a job" << std::endl;
            thisJob->doWork();
            --activeJobs;
        }));
    }
}

void ThreadPool::QueueJob(jobVariantPtr jobPtr) {
    pthread_mutex_lock(&queueMutex);
    jobs.push(jobPtr);
    pthread_mutex_unlock(&queueMutex);

}

bool ThreadPool::isBusy() {
    pthread_mutex_lock(&queueMutex);
    bool ret = !jobs.empty() || activeJobs > 0;
    pthread_mutex_unlock(&queueMutex);

    return ret;
    
}

void ThreadPool::Stop() {
    should_terminate = true;
    for (auto &future : futures) {
        future.get();
    }
}

void ThreadPool::kill() {
    should_terminate = true;
}

#endif /* ThreadPoolAsync_hpp */
