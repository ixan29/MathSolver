#include "ConstFunction.h"

const double& ConstFunction::getConstant() const {
	return constant;
}


ConstFunction::ConstFunction(const double& constant_)
:Function(typeid(ConstFunction).hash_code())
,constant(constant_)
{}

std::shared_ptr<Function> ConstFunction::create(const double& constant_) {
	return std::shared_ptr<ConstFunction>(new ConstFunction(constant_));
}

std::shared_ptr<Function> ConstFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables) {
	return ConstFunction::create(constant);
}

std::shared_ptr<Function> ConstFunction::derivative(std::string variable) {
	return create(0);
}

void ConstFunction::print(std::ostream& os) const
{
	os << constant;
}

bool ConstFunction::equals(const Function& other) const
{
	return other.getType() == typeid(ConstFunction).hash_code()
	&& static_cast<const ConstFunction&>(other).getConstant() == constant;
}
