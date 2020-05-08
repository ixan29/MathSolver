#include "SumFunction.h"
#include "ConstFunction.h"
#include "MulFunction.h"
#include "DivFunction.h"
#include "PowFunction.h"
#include "TrigFunctionsUtils.h"
#include <algorithm>
#include <iostream>
#include <queue>

std::vector<std::shared_ptr<Function>> SumFunction::getFunctions() {
	return functions;
}

SumFunction::SumFunction(const std::vector<std::shared_ptr<Function>>& functions_)
:Function(typeid(SumFunction).hash_code())
,functions(functions_)
{}

std::vector<std::shared_ptr<Function>> flatSumFunctions(const std::vector<std::shared_ptr<Function>>& functions)
{
	std::queue<std::shared_ptr<Function>> toCheck;

	for(auto& func : functions)
		toCheck.push(func);

	std::vector<std::shared_ptr<Function>> flat;

	for(;!toCheck.empty(); toCheck.pop())
		if(toCheck.front()->getType() == typeid(SumFunction).hash_code())
			for(auto& func : std::static_pointer_cast<SumFunction>(toCheck.front())->getFunctions())
				toCheck.push(func);
		else
			flat.push_back(toCheck.front());
		

	return flat;
}

int getIdxSameMul(
	const std::vector<
		std::pair<
			std::shared_ptr<Function>,
			double
		>
	>& simplified,
	const std::shared_ptr<Function>& function
) {
	for(size_t k=0 ; k<simplified.size(); k++)
		if(*simplified[k].first == *function)
			return k;

	return -1;
}

std::vector<std::shared_ptr<Function>> simplifyMultipliers(const std::vector<std::shared_ptr<Function>>& functions)
{
	std::vector<
		std::pair<
			std::shared_ptr<Function>,
			double
		>
	> simplified;

	for(auto& function : functions)
		if(function->getType()==typeid(MulFunction).hash_code())
		{
			auto mulFunction = std::static_pointer_cast<MulFunction>(function);
			
			auto noConst = mulFunction->removeConstant();

			int idx = getIdxSameMul(simplified, noConst);

			if(idx >= 0)
				simplified[idx].second += mulFunction->getConstantValue();
			else
			{
				simplified.push_back({
					noConst,
					mulFunction->getConstantValue()
				});
			}
		}
		else
		{
			int idx = getIdxSameMul(simplified, function);

			if(idx >= 0)
				simplified[idx].second += 1;
			else
				simplified.push_back({
					function,
					1
				});
		}
	
	std::vector<std::shared_ptr<Function>> merged;
	merged.reserve(simplified.size());

	for(auto& s : simplified)
		merged.push_back(
			MulFunction::create({
				s.first,
				ConstFunction::create(s.second)
			})
		);

	return merged;
}

std::vector<std::shared_ptr<Function>> simplifySumConstants(const std::vector<std::shared_ptr<Function>>& functions)
{
	double simple=0;

	std::vector<std::shared_ptr<Function>> noConstant;

	for(auto& function : functions)
		if(function->getType() == typeid(ConstFunction).hash_code())
			simple += std::static_pointer_cast<ConstFunction>(function)->getConstant();
		else
			noConstant.push_back(function);

	if(simple != 0)
		noConstant.push_back(ConstFunction::create(simple));

	return noConstant;
}

bool isDivisible(const std::vector<std::shared_ptr<Function>> functions, const std::shared_ptr<Function> divisor)
{
	for(auto& function : functions)
		if(function->getType() == typeid(MulFunction).hash_code())
		{
			for(auto& mul : std::static_pointer_cast<MulFunction>(function)->getFunctions())
				if(isDivisible({mul}, divisor))
					return true;

			return false;
		}
		else
		if(function->getType() == typeid(PowFunction).hash_code())
			return *std::static_pointer_cast<PowFunction>(function)->getBase() == *divisor;
		else
			return *function == *divisor;
}

const std::shared_ptr<Function> findCommonDivisor(const std::vector<std::shared_ptr<Function>>& functions)
{
	auto& first = functions[0];
	auto rest = std::vector<std::shared_ptr<Function>>(functions.begin()+1, functions.end());

	if(first->getType() == typeid(MulFunction).hash_code())
	{
		for(auto& function : std::static_pointer_cast<MulFunction>(first)->getFunctions())
		{
			if(function->getType() == typeid(PowFunction).hash_code())
			{
				auto base = std::static_pointer_cast<PowFunction>(function)->getBase();

				if(isDivisible(rest, base))
					return base;
			}
			else
			{
				if(isDivisible(rest, function))
					return function;
			}
		}
	}
	else
	if(first->getType() == typeid(PowFunction).hash_code())
	{
		auto base = std::static_pointer_cast<PowFunction>(first)->getBase();

		if(isDivisible(rest, base))
			return base;
	}
	else
	if(isDivisible(rest, first))
		return first;

	return ConstFunction::create(1);
}

std::vector<std::shared_ptr<Function>> factorize(const std::vector<std::shared_ptr<Function>>& functions)
{
	if(functions.size() <= 1)
		return functions;

	for(auto& function : functions)
		if(function->getType() == typeid(ConstFunction).hash_code())
			return functions;

	auto divisor = findCommonDivisor(functions);

	if(*divisor == *ConstFunction::create(1))
		return functions;

	std::vector<std::shared_ptr<Function>> divided;
	divided.reserve(functions.size());

	for(auto& function : functions)
		divided.push_back(DivFunction::create(function, divisor));

	return {
		MulFunction::create({
			divisor,
			SumFunction::create(divided)
		})
	};
}

std::shared_ptr<Function> SumFunction::create(const std::vector<std::shared_ptr<Function>>& functions_)
{
	auto functions = flatSumFunctions(functions_);
	functions = simplifyMultipliers(functions);
	functions = simplifySumConstants(functions);
	functions = factorize(functions);

	switch(functions.size())
	{
		case 0: return ConstFunction::create(0);
		case 1: return functions[0];
		default:
			auto function = std::shared_ptr<Function>(new SumFunction(functions));
			// function = checkTrigSimplifiable(function);
			return function;
	}		
}

std::shared_ptr<Function> SumFunction::createNoOptimize(const std::vector<std::shared_ptr<Function>>& functions_) {
	return std::shared_ptr<SumFunction>(new SumFunction(functions_));
}

std::shared_ptr<Function> SumFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables)
{
	std::vector<std::shared_ptr<Function>> solved;
	solved.reserve(functions.size());

	for(auto& function : functions)
		solved.push_back(function->solve(variables));

	return SumFunction::create(solved);
}

std::shared_ptr<Function> SumFunction::derivative(std::string variable)
{
	std::vector<std::shared_ptr<Function>> derivatives;
	derivatives.reserve(functions.size());

	for(auto& function : functions)
		derivatives.push_back(std::move(function->derivative(variable)));

	return SumFunction::create(derivatives);
}

void SumFunction::print(std::ostream& os) const
{
	os << *functions[0];

	for(size_t k=1;k<functions.size();k++)
	{
		if(functions[k]->getType() == typeid(ConstFunction).hash_code())
		{
			if(std::static_pointer_cast<ConstFunction>(functions[k])->getConstant()>=0)
				 os << " + ";
		}
		else
			os << " + ";

		os << *functions[k];
	}
}

bool SumFunction::equals(const Function& other) const
{
	if(other.getType() != typeid(SumFunction).hash_code())
		return false;

	auto& sumFunction = static_cast<const SumFunction&>(other);

	if(sumFunction.functions.size() != functions.size())
		return false;
	
	size_t count = 0;

	for(auto& f1 : functions)
		for(auto& f2 : sumFunction.functions)
			if(*f1 == *f2)
				count++;

	return count == functions.size();
}