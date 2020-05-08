#include "CosFunction.h"
#include "SinFunction.h"
#include "ConstFunction.h"
#include "MulFunction.h"

const std::shared_ptr<Function> CosFunction::getFunction() const {
	return function;
}

CosFunction::CosFunction(const std::shared_ptr<Function> function_)
:Function(typeid(CosFunction).hash_code())
,function(function_)
{}

std::shared_ptr<Function> CosFunction::create(const std::shared_ptr<Function> function_) {
	return std::shared_ptr<CosFunction>(new CosFunction(function_));
}

std::shared_ptr<Function> CosFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables)
{
		std::shared_ptr<Function> autoGen;

		return autoGen;
}

std::shared_ptr<Function> CosFunction::derivative(std::string variable)
{
	return
	MulFunction::create({
		ConstFunction::create(-1),
		SinFunction::create(function),
		function->derivative(variable)
	});
}

void CosFunction::print(std::ostream& os) const {
	os << "cos(" << *function << ")";
}

bool CosFunction::equals(const Function& other) const {
	return other.getType() == typeid(CosFunction).hash_code()
	&& *function == *static_cast<const CosFunction&>(other).getFunction();
}