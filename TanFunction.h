
#ifndef __TAN_FUNCTION__
#define __TAN_FUNCTION__

#include "Function.h"
#include "memory"

class TanFunction
{
public:
    static std::shared_ptr<Function> create(const std::shared_ptr<Function> function);
};

#endif /* __TAN_FUNCTION__ */