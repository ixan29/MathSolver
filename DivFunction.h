#ifndef __DIV_FUNCTION_H__
#define __DIV_FUNCTION_H__

#include "Function.h"
#include <memory>

class DivFunction
{
public:
    static std::shared_ptr<Function> create(std::shared_ptr<Function> divident, std::shared_ptr<Function> divider);
};

#endif /* __DIV_FUNCTION_H__ */