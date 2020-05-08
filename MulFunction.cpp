#include "SumFunction.h"
#include "MulFunction.h"
#include "PowFunction.h"
#include "Utils.h"

std::vector<std::shared_ptr<Function>> MulFunction::getFunctions() {
	return functions;
}

MulFunction::MulFunction(const std::vector<std::shared_ptr<Function>>& functions_)
:Function(typeid(MulFunction).hash_code())
,functions(functions_)
{}

std::vector<std::shared_ptr<Function>> flatMulFunctions(const std::vector<std::shared_ptr<Function>>& functions)
{
	std::queue<std::shared_ptr<Function>> toCheck;

	for(auto& func : functions)
		toCheck.push(func);

	std::vector<std::shared_ptr<Function>> flat;

	for(;!toCheck.empty(); toCheck.pop())
		if(toCheck.front()->getType() == typeid(PowFunction).hash_code())
		{
			auto powFunction = std::static_pointer_cast<PowFunction>(toCheck.front());

			if(powFunction->getBase()->getType() == typeid(MulFunction).hash_code())
				for(auto& mul :
					//flatMulFunctions (
						std::static_pointer_cast<MulFunction>(
							powFunction->getBase()
						)->getFunctions()
					//)
				)
					toCheck.push(
						PowFunction::create(
							mul,
							powFunction->getExponent()
						)
					);
			else
				flat.push_back(powFunction);
		}
		else
		if(toCheck.front()->getType() == typeid(MulFunction).hash_code())
			for(auto& func : std::static_pointer_cast<MulFunction>(toCheck.front())->getFunctions())
				toCheck.push(func);
		else
			flat.push_back(toCheck.front());

	return flat;
}

int getIdxSameBase(
	const std::vector<
		std::pair<
			std::shared_ptr<Function>,
			std::shared_ptr<Function>
		>
	>& simplified,
	const std::shared_ptr<Function>& function
) {
	for(size_t k=0 ; k<simplified.size(); k++)
		if(*simplified[k].first == *function)
			return k;

	return -1;
}

std::vector<std::shared_ptr<Function>> getSums(std::shared_ptr<Function> function)
{
	return
	function->getType() == typeid(SumFunction).hash_code()
	? std::static_pointer_cast<SumFunction>(function)->getFunctions()
	: std::vector<std::shared_ptr<Function>> {function};
}

std::vector<std::shared_ptr<Function>> getMuls(std::shared_ptr<Function> function)
{
	return
	function->getType() == typeid(MulFunction).hash_code()
	? std::static_pointer_cast<MulFunction>(function)->getFunctions()
	: std::vector<std::shared_ptr<Function>> {function};
}

bool shouldDistribute(std::vector<std::shared_ptr<Function>> functions)
{
	std::cout << "begin" << std::endl;
	for(auto f : functions)
		std::cout << *f << std::endl;
	std::cout << "end" << std::endl;

	for(size_t i=0 ; i<functions.size() ; i++)
		if(functions[i]->getType() == typeid(MulFunction).hash_code())
			functions[i] = std::static_pointer_cast<MulFunction>(functions[i])->removeConstant();

	for(size_t i=1 ; i<functions.size() ; i++)
		for(size_t j=0; j<i; j++)
			for(auto sumi : getSums(functions[i]))
				for(auto sumj : getSums(functions[j]))
					for(auto funci : getMuls(sumi))
						for(auto funcj : getMuls(sumj))
						{
							std::shared_ptr<Function> basei = funci;
							std::shared_ptr<Function> basej = funcj;
							float powi = 1;
							float powj = 1;

							if(funci->getType() == typeid(PowFunction).hash_code())
							{
								auto powFunci = std::static_pointer_cast<PowFunction>(funci);
								basei = powFunci->getBase();

								if(powFunci->getExponent()->getType() == typeid(ConstFunction).hash_code())
									powi = std::static_pointer_cast<ConstFunction>(powFunci->getExponent())->getConstant();
							}

							if(funcj->getType() == typeid(PowFunction).hash_code())
							{
								auto powFuncj = std::static_pointer_cast<PowFunction>(funcj);
								basej = powFuncj->getBase();

								if(powFuncj->getExponent()->getType() == typeid(ConstFunction).hash_code())
									powj = std::static_pointer_cast<ConstFunction>(powFuncj->getExponent())->getConstant();
							}

							if((*basei == *basej) && powi * powj < 0 && (powi >= -powj || powj >= -powi))
							{
								//std::cout << *basei << std::endl;
								//std::cout << powi << ", " << powj << std::endl;
								return true;
							}
						}

	return false;		
}

std::vector<std::shared_ptr<Function>> distribute (
	const std::vector<std::shared_ptr<Function>>& a,
	const std::vector<std::shared_ptr<Function>>& b
)
{
	std::vector<std::shared_ptr<Function>> distributed;
	distributed.reserve(a.size() * b.size());

	for(auto funcA : a)
		for(auto funcB : b)
			distributed.push_back(
				MulFunction::create({funcA, funcB})
			);

	return distributed;
}

std::vector<std::shared_ptr<Function>> distributeFunctions (const std::vector<std::shared_ptr<Function>>& functions)
{
	std::vector<std::shared_ptr<Function>> distributed {ConstFunction::create(1)};

	for(auto func : functions)
		distributed = distribute(
			distributed,
			func->getType() == typeid(SumFunction).hash_code()
			? std::static_pointer_cast<SumFunction>(func)->getFunctions()
			: std::vector<std::shared_ptr<Function>> {func}
		);
	
	return distributed;
}

std::vector<std::shared_ptr<Function>> simplifyPowers(const std::vector<std::shared_ptr<Function>>& functions)
{
	std::vector<
		std::pair<
			std::shared_ptr<Function>,
			std::shared_ptr<Function>
		>
	> simplified;

	for(auto& function : functions)
		if(function->getType()==typeid(PowFunction).hash_code())
		{
			auto powFunction = std::static_pointer_cast<PowFunction>(function);

			int idx = getIdxSameBase(simplified, powFunction->getBase());

			if(idx >= 0)
				simplified[idx].second =
				SumFunction::create({
					simplified[idx].second,
					powFunction->getExponent()
				});
			else
				simplified.push_back({
					powFunction->getBase(),
					powFunction->getExponent()
				});
		}
		else
		{
			int idx = getIdxSameBase(simplified, function);

			if(idx >= 0)
			{
				simplified[idx].second =
				SumFunction::create({
					simplified[idx].second,
					ConstFunction::create(1)
				});
			}
			else
				simplified.push_back({
					function,
					ConstFunction::create(1)
				});
		}

	std::vector<std::shared_ptr<Function>> merged;
	merged.reserve(simplified.size());

	for(auto& s : simplified)
		merged.push_back(
			PowFunction::create(
				s.first,
				s.second
			)
		);

	return merged;
}

std::vector<std::shared_ptr<Function>> simplifyMulConstants(const std::vector<std::shared_ptr<Function>>& functions)
{
	double simple=1;

	std::vector<std::shared_ptr<Function>> noConstant;

	for(auto& function : functions)
		if(function->getType() == typeid(ConstFunction).hash_code())
			simple *= std::static_pointer_cast<ConstFunction>(function)->getConstant();
		else
			noConstant.push_back(function);

	if(simple == 0)
		return {ConstFunction::create(0)};

	if(simple != 1)
		noConstant.push_back(ConstFunction::create(simple));

	return noConstant;
}

std::shared_ptr<Function> MulFunction::create(const std::vector<std::shared_ptr<Function>>& functions_)
{
	/*
	std::cout << "begin" << std::endl;
	for(auto func : functions_)
		std::cout << *func << std::endl;
	std::cout << "end" << std::endl;
	*/
	auto functions = flatMulFunctions(functions_);
	functions = simplifyMulConstants(functions);
	functions = simplifyPowers(functions);
	functions = simplifyMulConstants(functions);

	switch(functions.size())
	{
		case 0: return ConstFunction::create(1);
		case 1: return functions[0];
		default:
			if(shouldDistribute(functions))
			{
				auto dist = distributeFunctions(functions);
				return SumFunction::create(dist);
			}
			else
				return std::shared_ptr<MulFunction>(new MulFunction(functions));
	}	
}

std::shared_ptr<Function> MulFunction::createNoOptimize(const std::vector<std::shared_ptr<Function>>& functions_) {
	return std::shared_ptr<MulFunction>(new MulFunction(functions_));
}

std::shared_ptr<Function> MulFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables)
{
	std::vector<std::shared_ptr<Function>> solved;
	solved.reserve(functions.size());

	for(auto& function : functions)
		solved.push_back(function->solve(variables));

	return MulFunction::create(solved);
}

std::shared_ptr<Function> MulFunction::derivative(std::string variable)
{
	std::vector<std::shared_ptr<Function>> sums;
	sums.resize(functions.size());

	std::vector<std::shared_ptr<Function>> muls;
	muls.resize(functions.size());

	for(size_t i=0 ; i<functions.size() ; i++)
	{
		for(size_t j=0 ; j<functions.size() ; j++)
			muls[j] = std::move(
				i==j
				? functions[j]->derivative(variable)
				: functions[j]
			);

		sums[i] = MulFunction::create(muls);
	}

	return SumFunction::create(sums);
}

std::shared_ptr<Function> MulFunction::removeConstant() const
{
	std::vector<std::shared_ptr<Function>> elems;
	elems.reserve(functions.size());

	for(auto func : functions)
		if(func->getType() != typeid(ConstFunction).hash_code())
			elems.push_back(func);

	return createNoOptimize(elems);
}

void MulFunction::print(std::ostream& os) const
{
	if(needsParentheses(*functions[0]))
		os << "(" << *functions[0] << ")";
	else
		os << *functions[0];

	for(size_t k=1 ; k<functions.size() ; k++)
	{
		os << " * ";
		if(needsParentheses(*functions[k]))
			os << "(" << *functions[k] << ")";
		else
			os << *functions[k];
	}
}

bool MulFunction::equals(const Function& other) const {
	
	if(other.getType() != typeid(MulFunction).hash_code())
		return false;

	auto& mulFunction = static_cast<const MulFunction&>(other);

	if(mulFunction.functions.size() != functions.size())
		return false;
	
	size_t count = 0;

	for(auto& f1 : functions)
		for(auto& f2 : mulFunction.functions)
			if(*f1 == *f2)
				count++;

	return count == functions.size();
}

bool MulFunction::equalsIgnoreConstant(const MulFunction& other) const
{
	size_t count = 0;

	for(auto& f1 : functions)
		for(auto& f2 : other.functions)
			if((*f1 == *f2)
			|| (
				f1->getType() == typeid(ConstFunction).hash_code()
			&&	f2->getType() == typeid(ConstFunction).hash_code()
			))
				count++;

	return count == functions.size();
}

double MulFunction::getConstantValue() const
{
	for(auto& function : functions)
		if(function->getType() == typeid(ConstFunction).hash_code())
			return std::static_pointer_cast<ConstFunction>(function)->getConstant();

	return 1;
}