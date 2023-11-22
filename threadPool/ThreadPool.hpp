// ThreadPool.hpp
//
// ThreadPool using std::thread and std::mutex
//
// Created on 10/26/23.

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "definitions.hpp"
#include "JobClass.hpp"
#include "StringJob.hpp"
#include "IntJob.hpp"

class ThreadPool {
public:
    ThreadPool();
    explicit ThreadPool(size_t threads);
    ~ThreadPool();
    void Start();
    void QueueJob(jobVariantPtr job);
    void Stop();
    bool isBusy();

private:
    bool should_terminate = false;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::queue<jobVariantPtr> jobs;
    std::vector<std::thread> workers;
    std::atomic<int> activeJobs{0};

    void WorkerThread();
    void InitializeThreads(size_t threads);

};

// Default constructor that initializes threads to the hardware concurrency limit
ThreadPool::ThreadPool() : ThreadPool(std::thread::hardware_concurrency()) {}

// Constructor that allows specifying a number of threads
ThreadPool::ThreadPool(size_t threads) {
    InitializeThreads(threads);
}

void ThreadPool::InitializeThreads(size_t threads) {
    auto max_threads = std::thread::hardware_concurrency();
    if (threads > max_threads) {
        throw std::invalid_argument("Number of threads requested exceeds hardware concurrency limit.");
    }

    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back(&ThreadPool::WorkerThread, this);
    }
}

ThreadPool::~ThreadPool() {
    Stop();
}

void ThreadPool::Start() {
    // All threads are started in the constructor
}

void ThreadPool::WorkerThread() {
    while (true) {
        jobVariantPtr job;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]() {
                return should_terminate || !jobs.empty();
            });
            if (should_terminate && jobs.empty()) {
                return;
            }
            job = std::move(jobs.front());
            jobs.pop();
        }
        ++activeJobs;
        job->doWork();
        --activeJobs;
    }
}

void ThreadPool::QueueJob(jobVariantPtr jobPtr) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        jobs.push(std::move(jobPtr));
    }
    condition.notify_one();
}

bool ThreadPool::isBusy() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return !jobs.empty() || activeJobs > 0;
}

void ThreadPool::Stop() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        should_terminate = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

#endif /* ThreadPool_hpp */
