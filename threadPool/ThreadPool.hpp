//
//  ThreadPool.hpp
//  threadPool
//
//  Created by josh on 10/26/23.
//

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <stdio.h>
#include <thread>
#include <mutex>
#include <queue>
#include <memory>

#include "definitions.hpp"


class ThreadPool{
public:
    void Start();
    void QueueJob(jobVariantPtr job);
    void Stop();
    bool isBusy();
    
    void kill();
    
    
    
private:
    bool busy;
    uint64_t jobId;
    
    void ThreadLoop();
    
    bool should_terminate = false;
    std::mutex queueMutex;
    std::condition_variable mutex_condition;
    std::vector<std::thread> threads;
    std::queue<jobVariantPtr> jobs;
    
};

#endif /* ThreadPool_hpp */
