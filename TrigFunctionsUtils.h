
#ifndef __TRIG_FUNCTIONS_UTILS__
#define __TRIG_FUNCTIONS_UTILS__

#include <vector>
#include <memory>
#include "Function.h"
#include "SinFunction.h"
#include "CosFunction.h"

std::shared_ptr<Function> checkTrigSimplifiable(std::shared_ptr<Function> function);

#endif /* __TRIG_FUNCTIONS_UTILS__ */