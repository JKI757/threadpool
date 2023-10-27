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


class ThreadPool{
public:
    void Start();
    void QueueJob(const std::function<void()>& job);
    void Stop();
    bool isBusy();
    uint64_t getJobId();
    void setJobId(uint64_t id);
    
    
    
private:
    bool busy;
    uint64_t jobId;
    
    void ThreadLoop();
    
    bool should_terminate = false;
    std::mutex queueMutex;
    std::condition_variable mutex_condition;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
    
};

#endif /* ThreadPool_hpp */
