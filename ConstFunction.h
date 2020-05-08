#ifndef __CONST_FUNCTION_H__
#define __CONST_FUNCTION_H__

#include <string>
#include <map>
#include <memory>

#include "Function.h"

/**
 * ConstFunction class
 * 
 * TODO add description
**/
class ConstFunction: public Function
{
	double constant;

public: 
	/**
	 * Gets the constant.
	 * @return the constant
	**/
	const double& getConstant() const;


private:
	/**
	 * const function constructor
	 * @param constant initial value of constant
	**/
	ConstFunction(const double& constant_);
public:

	static std::shared_ptr<Function> create(const double& constant_);

	/**
	 *  const function virtual destructor
	**/
	virtual ~ConstFunction() {}

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

#endif /* __CONST_FUNCTION_H__ */

