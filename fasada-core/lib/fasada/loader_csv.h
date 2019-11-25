#ifndef PROCESSOR_CSV_H
#define PROCESSOR_CSV_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "fasada.hpp"
#include "tree_processor.h"

namespace fasada
{


class loader_csv : public tree_processor
{
public:
    loader_csv(const char* name="csv");
   ~loader_csv(){}
};

}
#endif // PROCESSOR_CSV_H