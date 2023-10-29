//
//  ThreadPool.cpp
//  threadPool
//
//  Created by josh on 10/26/23.
//

#include "ThreadPool.hpp"

void ThreadPool::Start(){
    const uint32_t numThreads = std::thread::hardware_concurrency()/4; //max threads machine can handle.
    for (uint32_t ii = 0; ii < numThreads; ++ii){
        threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }
}

void ThreadPool::ThreadLoop(){
    while(true){
        std::shared_ptr<dataClass> job;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            mutex_condition.wait(lock, [this]{
                return !jobs.empty() || should_terminate;
            });
            if (should_terminate){
                return;
            }
            job = jobs.front();
            jobs.pop();
            job->printSomething();

        }
    }
}


void ThreadPool::QueueJob(const std::shared_ptr<dataClass> job, int data){
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        job->setData(data);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}//to use: thread_pool->QueueJob([] { /* ... */ });


bool ThreadPool::isBusy(){
    bool poolBusy;
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        poolBusy = !jobs.empty();
    }
    return poolBusy;
}

void ThreadPool::Stop(){
    {
        std::unique_lock<std::mutex>lock(queueMutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for(std::thread& activeThread : threads){
        activeThread.join();
    }
    threads.clear();
}


void ThreadPool::kill(){
    this->should_terminate = true;
}
