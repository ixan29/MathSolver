#include "TanFunction.h"
#include "DivFunction.h"
#include "SinFunction.h"
#include "CosFunction.h"

std::shared_ptr<Function> TanFunction::create(const std::shared_ptr<Function> function)
{
    return
    DivFunction::create (
        SinFunction::create(function),
        CosFunction::create(function)
    );
}