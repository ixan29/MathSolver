#include "SinFunction.h"
#include "CosFunction.h"
#include "MulFunction.h"

const std::shared_ptr<Function> SinFunction::getFunction() const {
	return function;
}

SinFunction::SinFunction(const std::shared_ptr<Function> function_)
:Function(typeid(SinFunction).hash_code())
,function(function_)
{}

std::shared_ptr<Function> SinFunction::create(const std::shared_ptr<Function> function_) {
	return std::shared_ptr<SinFunction>(new SinFunction(function_));
}

std::shared_ptr<Function> SinFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables) {
	return create(function->solve(variables));
}

std::shared_ptr<Function> SinFunction::derivative(std::string variable)
{
	return
	MulFunction::create({
		CosFunction::create(function),
		function->derivative(variable)
	});
}

void SinFunction::print(std::ostream& os) const {
	os << "sin(" << *function << ")";
}

bool SinFunction::equals(const Function& other) const {
	return other.getType() == typeid(SinFunction).hash_code()
	&& *function == *static_cast<const SinFunction&>(other).getFunction();
}