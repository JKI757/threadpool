//
//  main.cpp
//  threadPool
//
//  Created by josh on 10/26/23.
//

#include <iostream>
#include "ThreadPool.hpp"
#include "StringJob.hpp"
#include "IntJob.hpp"
#include "JobClass.hpp"
#include "definitions.hpp"
#include <unistd.h>
#include <sstream>


int main(int argc, const char * argv[]) {
    // insert code here...
    ThreadPool n;
    n.Start();
    for(int ii=0; ii < 5000; ++ii){
        usleep(2000);
        jobVariantPtr job = std::dynamic_pointer_cast<JobClass> (std::make_shared<IntJob> (ii));
        n.QueueJob(job);
        std::stringstream s;
        s << "the number is: " << ii;
        job = std::dynamic_pointer_cast<JobClass> (std::make_shared<StringJob> (s.str()));
        n.QueueJob(job);

    }
    while(n.isBusy()){;}
    n.Stop();
    std::cout << "Hello, World!\n";
    return 0;
}
