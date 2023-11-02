//
//  JobClass.hpp
//  threadPool
//
//  Created by josh on 10/31/23.
//

#ifndef JobClass_h
#define JobClass_h


// Base class for the Job class hierarchy
class JobClass {
public:
    virtual void doWork() = 0;
    virtual ~JobClass() = default;
};

#endif /* JobClass_h */
