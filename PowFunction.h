#ifndef __POW_FUNCTION_H__
#define __POW_FUNCTION_H__

#include <string>
#include <map>
#include <memory>

#include "Function.h"

/**
 * PowFunction class
 * 
 * TODO add description
**/
class PowFunction: public Function
{
	std::shared_ptr<Function> base;
	std::shared_ptr<Function> exponent;

public:
	std::shared_ptr<Function> getBase() const;
	std::shared_ptr<Function> getExponent() const;

private:

	/**
	 * pow function constructor
	 * @param exponent initial value of exponent
	**/
	PowFunction(const std::shared_ptr<Function> base_, const std::shared_ptr<Function> exponent_);
public:
	static std::shared_ptr<Function> create(const std::shared_ptr<Function> base_, const std::shared_ptr<Function> exponent_);

	/**
	 *  pow function virtual destructor
	**/
	virtual ~PowFunction() {}

	/**
	 * TODO complete method documentation
	 * @param variables
	 * @return
	**/
	virtual std::shared_ptr<Function> solve(std::map<std::string,std::shared_ptr<Function>>& variables);
	/**
	 * TODO complete method documentation
	 * @param variable
	 * @return
	**/
	virtual std::shared_ptr<Function> derivative(std::string variable);

	virtual void print(std::ostream& os) const;
	virtual bool equals(const Function& other) const;
};

#endif /* __POW_FUNCTION_H__ */

