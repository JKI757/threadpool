//
//  JobClass.hpp
//  threadPool
//
//  Created by josh on 10/31/23.
//

#ifndef JobClass_h
#define JobClass_h

#include "data.h"
#include "Globals.hpp"

// Base class for the Job class hierarchy
class JobClass {
public:
    JobClass(Globals& g) : global(g){};
    virtual void doWork() = 0;
    virtual ~JobClass() = default;
    
private:
    Globals& global;
};

#endif /* JobClass_h */
