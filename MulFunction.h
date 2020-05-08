#ifndef __MUL_FUNCTION_H__
#define __MUL_FUNCTION_H__

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Function.h"

/**
 * MulFunction class
 * 
 * TODO add description
**/
class MulFunction: public Function
{
	std::vector<std::shared_ptr<Function>> functions;

public:
	std::vector<std::shared_ptr<Function>> getFunctions();

private:
	/**
	 * mul function constructor
	 * @param functions initial value of functions
	**/
	MulFunction(const std::vector<std::shared_ptr<Function>>& functions_);
public:

	static std::shared_ptr<Function> create(const std::vector<std::shared_ptr<Function>>& functions_);
	static std::shared_ptr<Function> createNoOptimize(const std::vector<std::shared_ptr<Function>>& functions_);

	/**
	 *  mul function virtual destructor
	**/
	virtual ~MulFunction() {}

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

	std::shared_ptr<Function> removeConstant() const;

	virtual void print(std::ostream& os) const;
	virtual bool equals(const Function& other) const;

	bool equalsIgnoreConstant(const MulFunction& other) const;
	double getConstantValue() const;
};

#endif /* __MUL_FUNCTION_H__ */

