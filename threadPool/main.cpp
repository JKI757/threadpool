//
//  main.cpp
//  threadPool
//
//  Created by josh on 10/26/23.
//

#include <iostream>
#include "ThreadPool.hpp"
#include <unistd.h>
#include <sstream>


void printSomething(int i){
    std::cout << "I'm printing!!" << i << std::endl;
}
int main(int argc, const char * argv[]) {
    // insert code here...
    ThreadPool n;
    n.Start();
    std::function<void(int i)> func = printSomething;
    for(int ii=0; ii < 5000; ++ii){
        usleep(2000);
        jobVariant job = IntJob(ii);
        jobVariantPtr jobPtr = std::make_shared<jobVariant>(job);
        n.QueueJob(jobPtr);
        std::stringstream s;
        s << "the number is: " << ii << std::endl;
        job = StringJob(s.str());
        jobPtr = std::make_shared<jobVariant>(job);
        n.QueueJob(jobPtr);
        
    }
    while(n.isBusy()){;}
    n.Stop();
    std::cout << "Hello, World!\n";
    return 0;
}
