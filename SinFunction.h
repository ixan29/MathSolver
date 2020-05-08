#ifndef __SIN_FUNCTION_H__
#define __SIN_FUNCTION_H__

#include <string>
#include <map>
#include <memory>

#include "Function.h"

/**
 * SinFunction class
 * 
 * TODO add description
**/
class SinFunction: public Function
{
	std::shared_ptr<Function> function;

public: 
	/**
	 * Gets the function.
	 * @return the function
	**/
	const std::shared_ptr<Function> getFunction() const;


private:
	/**
	 * sin function constructor
	 * @param function initial value of function
	**/
	SinFunction(const std::shared_ptr<Function> function_);
public:

	static std::shared_ptr<Function> create(const std::shared_ptr<Function> function_);

	/**
	 *  sin function virtual destructor
	**/
	virtual ~SinFunction() {}

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

#endif /* __SIN_FUNCTION_H__ */

