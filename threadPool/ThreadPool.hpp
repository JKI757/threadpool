// ThreadPool.hpp
// threadPool
//
// Created by josh on 10/26/23.

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <stdio.h>
#include <vector>
#include <queue>
#include <memory>
#include <pthread.h>
#include "JobClass.hpp"
#include "definitions.hpp"

class ThreadPool {
public:
    ThreadPool(uint16_t maxThreads=4) {
        pthread_mutex_init(&queueMutex, NULL);
        pthread_cond_init(&mutex_condition, NULL);
        this->maxThreads = maxThreads;
    }
    ~ThreadPool() {
        pthread_mutex_destroy(&queueMutex);
        pthread_cond_destroy(&mutex_condition);
    }

    void Start();
    void QueueJob(jobVariantPtr job);
    void Stop();
    bool isBusy();
    void kill();

private:
    bool should_terminate = false;
    pthread_mutex_t queueMutex;
    pthread_cond_t mutex_condition;
    std::vector<pthread_t> threads;
    std::queue<jobVariantPtr> jobs;
    uint16_t maxThreads;
    static void* ThreadLoop(void* arg);
};

// Function definitions

void ThreadPool::Start() {
    const uint32_t numThreads = 4;
    threads.resize(numThreads);
    for (uint32_t ii = 0; ii < numThreads; ++ii) {
        pthread_create(&threads[ii], NULL, &ThreadPool::ThreadLoop, this);
    }
}

void* ThreadPool::ThreadLoop(void* arg) {
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    while (true) {
        jobVariantPtr thisJob;
        pthread_mutex_lock(&(pool->queueMutex));
        while (pool->jobs.empty() && !pool->should_terminate) {
            pthread_cond_wait(&(pool->mutex_condition), &(pool->queueMutex));
        }
        if (pool->should_terminate) {
            pthread_mutex_unlock(&(pool->queueMutex));
            pthread_exit(NULL);
        }
        thisJob = pool->jobs.front();
        pool->jobs.pop();
        pthread_mutex_unlock(&(pool->queueMutex));
        thisJob->doWork();
    }
    return NULL;
}

void ThreadPool::QueueJob(jobVariantPtr jobPtr) {
    pthread_mutex_lock(&queueMutex);
    jobs.push(jobPtr);
    pthread_mutex_unlock(&queueMutex);
    pthread_cond_signal(&mutex_condition);
}

bool ThreadPool::isBusy() {
    pthread_mutex_lock(&queueMutex);
    bool poolBusy = !jobs.empty();
    pthread_mutex_unlock(&queueMutex);
    return poolBusy;
}

void ThreadPool::Stop() {
    pthread_mutex_lock(&queueMutex);
    should_terminate = true;
    pthread_mutex_unlock(&queueMutex);
    pthread_cond_broadcast(&mutex_condition);
    for (uint32_t ii = 0; ii < threads.size(); ++ii) {
        pthread_join(threads[ii], NULL);
    }
}

void ThreadPool::kill() {
    should_terminate = true;
}

#endif /* ThreadPool_hpp */
