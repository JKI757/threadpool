//
//  IntJob.hpp
//  threadPool
//
//  Created by josh on 10/31/23.
//

#ifndef IntJob_h
#define IntJob_h
#include <iostream>

class IntJob : public JobClass {
public:
    IntJob(const int& data, Globals& global) : JobClass(global), data(data) {}

    void doWork() override {
        // Perform operations specific to IntJob
        std::cout << "Processing IntJob with data: " << data << std::endl;
    }

private:
    int data;
};

#endif /* IntJob_h */
