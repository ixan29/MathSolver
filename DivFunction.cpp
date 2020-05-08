#include "DivFunction.h"
#include "ConstFunction.h"
#include "MulFunction.h"
#include "PowFunction.h"

std::shared_ptr<Function> DivFunction::create(std::shared_ptr<Function> divident, std::shared_ptr<Function> divider)
{
    return
    MulFunction::create({
        divident,
        PowFunction::create(
            divider,
            ConstFunction::create(-1)
        )
    });
}