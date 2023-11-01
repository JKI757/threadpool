//
//  definitions.hpp
//  threadPool
//
//  Created by josh on 10/31/23.
//

#ifndef definitions_h
#define definitions_h

#include <variant>
#include <memory>
#include "JobClass.hpp"
#include "StringJob.hpp"
#include "IntJob.hpp"

typedef std::variant<IntJob, StringJob> jobVariant;

typedef  std::shared_ptr<jobVariant> jobVariantPtr;


#endif /* definitions_h */
