#ifndef __SUM_FUNCTION_H__
#define __SUM_FUNCTION_H__

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Function.h"

/**
 * SumFunction class
 * 
 * TODO add description
**/
class SumFunction: public Function
{
	std::vector<std::shared_ptr<Function>> functions;

public:
	std::vector<std::shared_ptr<Function>> getFunctions();

private:
	/**
	 * sum function constructor
	 * @param functions initial value of functions
	**/
	SumFunction(const std::vector<std::shared_ptr<Function>>& functions_);
public:

	static std::shared_ptr<Function> create(const std::vector<std::shared_ptr<Function>>& functions_);
	static std::shared_ptr<Function> createNoOptimize(const std::vector<std::shared_ptr<Function>>& functions_);
	/**
	 *  sum function virtual destructor
	**/
	virtual ~SumFunction() {}

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

#endif /* __SUM_FUNCTION_H__ */

