#include "VarFunction.h"
#include "ConstFunction.h"

const std::string& VarFunction::getVariable() const {
	return variable;
}


VarFunction::VarFunction(const std::string& variable_)
:Function(typeid(VarFunction).hash_code())
,variable(variable_)
{}

std::shared_ptr<Function> VarFunction::create(const std::string& variable_){
	return std::shared_ptr<VarFunction>(new VarFunction(variable_));
}

std::shared_ptr<Function> VarFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables)
{
	if(variables.count(variable))
		return variables[variable];
	else
		return VarFunction::create(variable);
}

std::shared_ptr<Function> VarFunction::derivative(std::string variable_) {
	return ConstFunction::create(variable == variable_);
}

void VarFunction::print(std::ostream& os) const
{
	os << variable;
}

bool VarFunction::equals(const Function& other) const
{
	return other.getType() == typeid(VarFunction).hash_code()
	&& static_cast<const VarFunction&>(other).getVariable() == variable;
}