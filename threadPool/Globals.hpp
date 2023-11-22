//
//  Globals.hpp
//  threadPool
//
//  Created by josh on 11/8/23.
//

#ifndef Globals_hpp
#define Globals_hpp

#include <stdio.h>
#include <memory>
#include <queue>
#include <mutex>

#include "data.h"


class Globals{
  
public:
    Globals(){}
    
    ~Globals(){}
    
    
    std::shared_ptr<data> getTopData(){std::shared_ptr<data> d = std::move(theDataQueue.front()); theDataQueue.pop_front(); return d;}
    void insertData(std::shared_ptr<data> d){this->theDataQueue.push_back(std::move(d));}
    
private:
    
    std::deque<std::shared_ptr<data>> theDataQueue;
    std::mutex dqMutex;
    
};
#endif /* Globals_hpp */
