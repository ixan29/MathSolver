
#include "TrigFunctionsUtils.h"
#include "ConstFunction.h"
#include "SumFunction.h"
#include "MulFunction.h"
#include "PowFunction.h"
#include "map"
#include "functional"

std::vector<
    std::pair<
        std::function<
            std::shared_ptr<Function>
            (std::shared_ptr<Function>)
        >,
        std::function<
            std::shared_ptr<Function>
            (std::shared_ptr<Function>)
        >
    >
> trigReplacements
{
    {
        [] (std::shared_ptr<Function> function) {
            return
            SumFunction::createNoOptimize({
                PowFunction::create(
                    CosFunction::create(function),
                    ConstFunction::create(2)
                ),
                PowFunction::create(
                    SinFunction::create(function),
                    ConstFunction::create(2)
                )
            });
        },
        [] (std::shared_ptr<Function> function) {
            return ConstFunction::create(1);
        }
    }
};

std::shared_ptr<Function> getTrigSubFunction(const std::shared_ptr<SumFunction> function)
{
    for(auto func : function->getFunctions())
    {
        std::shared_ptr<PowFunction> pow(nullptr);

        if(func->getType() == typeid(PowFunction).hash_code())
            pow = std::static_pointer_cast<PowFunction>(func);
        else
        if(func->getType() == typeid(MulFunction).hash_code())
            for(auto func2 : std::static_pointer_cast<MulFunction>(func)->getFunctions())
                if(func2->getType() == typeid(PowFunction).hash_code())
                    pow = std::static_pointer_cast<PowFunction>(func2);

        if(*pow->getExponent() != *ConstFunction::create(2))
            return std::shared_ptr<Function>(nullptr);

        if(pow->getBase()->getType() == typeid(SinFunction).hash_code())
            return std::static_pointer_cast<SinFunction>(pow->getBase())->getFunction();

        if(pow->getBase()->getType() == typeid(CosFunction).hash_code())
            return std::static_pointer_cast<CosFunction>(pow->getBase())->getFunction();
    }

    return std::shared_ptr<Function>(nullptr);
}

std::shared_ptr<Function> checkTrigSimplifiable(std::shared_ptr<Function> function)
{
    if(function->getType() != typeid(SumFunction).hash_code())
        return function;

    auto sumFunction = std::static_pointer_cast<SumFunction>(function);
    
    if(sumFunction->getFunctions().size() != 2)
        return function;

    auto sub = getTrigSubFunction(sumFunction);

    if(sub == nullptr)
        return function;

    for(auto& replacement : trigReplacements)
        if(*replacement.first(sub) == *function)
            return replacement.second(sub);

    return function;
}