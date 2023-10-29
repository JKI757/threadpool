//
//  dataClass.cpp
//  threadPool
//
//  Created by josh on 10/28/23.
//

#include "dataClass.hpp"

dataClass::dataClass(int i):data{i}{}

void dataClass::printSomething(){
    std::cout << "printing: " << this->data << std::endl;
}
void dataClass::setData(int i){this->data = i;}
int dataClass::getData(){return this->data;}
