#include "PowFunction.h"
#include "ConstFunction.h"
#include "SumFunction.h"
#include "MulFunction.h"
#include "LogFunction.h"
#include "Utils.h"
#include <cmath>

std::shared_ptr<Function> PowFunction::getBase() const {
	return base;
}

std::shared_ptr<Function> PowFunction::getExponent() const {
	return exponent;
}

PowFunction::PowFunction(const std::shared_ptr<Function> base_, const std::shared_ptr<Function> exponent_)
:Function(typeid(PowFunction).hash_code())
,base(base_)
,exponent(exponent_)
{}

std::shared_ptr<Function> PowFunction::create(const std::shared_ptr<Function> base_, const std::shared_ptr<Function> exponent_)
{
	if(base_->getType()==typeid(ConstFunction).hash_code()
	&& exponent_->getType()==typeid(ConstFunction).hash_code())
		return 
		ConstFunction::create(
			pow (
				std::static_pointer_cast<ConstFunction>(base_)->getConstant(),
				std::static_pointer_cast<ConstFunction>(exponent_)->getConstant()
			)
		);

	if(exponent_->getType()==typeid(ConstFunction).hash_code())
		if(std::static_pointer_cast<ConstFunction>(exponent_)->getConstant()==0)
			return ConstFunction::create(1);
		else
		if(std::static_pointer_cast<ConstFunction>(exponent_)->getConstant()==1)
			return base_;
		
	if(base_->getType()==typeid(PowFunction).hash_code())
		return
		PowFunction::create(
			std::static_pointer_cast<PowFunction>(base_)->getBase(),
			MulFunction::create({
				std::static_pointer_cast<PowFunction>(base_)->getExponent(),
				exponent_
			})
		);

	if(exponent_->getType()==typeid(ConstFunction).hash_code()
	&& std::static_pointer_cast<ConstFunction>(exponent_)->getConstant() == 1)
		return base_;

	if(base_->getType()==typeid(ConstFunction).hash_code())
		if(std::static_pointer_cast<ConstFunction>(base_)->getConstant()==0)
			return ConstFunction::create(0);
		else
		if(std::static_pointer_cast<ConstFunction>(base_)->getConstant()==1)
			return ConstFunction::create(1);

	return std::shared_ptr<PowFunction>(new PowFunction(base_, exponent_));
}

std::shared_ptr<Function> PowFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables)
{
	return
	PowFunction::create (
		base->solve(variables),
		exponent->solve(variables)
	);
}

std::shared_ptr<Function> PowFunction::derivative(std::string variable)
{
	return
	MulFunction::create({
		PowFunction::create(
			base,
			SumFunction::create({
				exponent,
				ConstFunction::create(-1)
			})
		),
		SumFunction::create({
			MulFunction::create({
				base->derivative(variable),
				exponent
			}),
			MulFunction::create({
				base,
				LogFunction::create(base),
				exponent->derivative(variable)
			})
		})
	});
}

void PowFunction::print(std::ostream& os) const
{
	os
	<< "(" << *base << ")"
	<< "^"
	<< "(" << *exponent << ")";
}

bool PowFunction::equals(const Function& other) const
{
	return other.getType() == typeid(PowFunction).hash_code()
	&& *static_cast<const PowFunction&>(other).getBase() == *base
	&& *static_cast<const PowFunction&>(other).getExponent() == *exponent;
}