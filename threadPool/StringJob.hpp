//
//  StringJob.hpp
//  threadPool
//
//  Created by josh on 10/31/23.
//

#ifndef StringJob_h
#define StringJob_h
#include "JobClass.hpp"
#include <iostream>

class StringJob : public JobClass {
public:
    StringJob(const std::string& data, Globals& global) : JobClass(global), data(data) {}
    void doWork() override {
        // Perform operations specific to StringJob
        std::cout << "Processing StringJob with data: " << data << std::endl;
    }

private:
    std::string data;
};

#endif /* StringJob_h */
