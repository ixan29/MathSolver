#include "ConstFunction.h"
#include "MulFunction.h"
#include "DivFunction.h"
#include "PowFunction.h"
#include "LogFunction.h"
#include "cmath"

std::shared_ptr<Function> LogFunction::getFunction() const {
	return function;
}

LogFunction::LogFunction(const std::shared_ptr<Function> function_)
:Function(typeid(LogFunction).hash_code())
,function(function_)
{}

std::shared_ptr<Function> LogFunction::create(const std::shared_ptr<Function> function) {

	if(function->getType() == typeid(PowFunction).hash_code())
		return
		MulFunction::create({
			std::static_pointer_cast<PowFunction>(function)->getExponent(),
			LogFunction::create(
				std::static_pointer_cast<PowFunction>(function)->getBase()
			)
		});

	if(function->getType() == typeid(ConstFunction).hash_code()
	&& std::static_pointer_cast<ConstFunction>(function)->getConstant()==1)
		return ConstFunction::create(0);

	return std::shared_ptr<LogFunction>(new LogFunction(function));
}

std::shared_ptr<Function> LogFunction::create(const std::shared_ptr<Function> logarithm, const std::shared_ptr<Function> function)
{
	return
	DivFunction::create (
		LogFunction::create(function),
		LogFunction::create(logarithm)
	);
}

std::shared_ptr<Function> LogFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables) {
	return LogFunction::create((function->solve(variables)));
}

std::shared_ptr<Function> LogFunction::derivative(std::string variable)
{
	return
	DivFunction::create(
		function->derivative(variable),
		function
	);
}

void LogFunction::print(std::ostream& os) const
{
	os << "log(" << *function << ")";
}

bool LogFunction::equals(const Function& other) const
{
	return other.getType() == typeid(LogFunction).hash_code()
	&& *static_cast<const LogFunction&>(other).getFunction() == *function;
}