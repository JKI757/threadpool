//
//  dataClass.hpp
//  threadPool
//
//  Created by josh on 10/28/23.
//

#ifndef dataClass_hpp
#define dataClass_hpp

#include <stdio.h>
#include <iostream>

class dataClass{
public:
    dataClass(int i);
    void printSomething();
    void setData(int i);
    int getData();
    
private:
    int data;
};
#endif /* dataClass_hpp */
