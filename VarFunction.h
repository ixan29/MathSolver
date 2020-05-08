#ifndef __VAR_FUNCTION_H__
#define __VAR_FUNCTION_H__

#include <string>
#include <map>
#include <memory>

#include "Function.h"

/**
 * VarFunction class
 * 
 * TODO add description
**/
class VarFunction: public Function
{
	std::string variable;

public: 
	/**
	 * Gets the variable.
	 * @return the variable
	**/
	const std::string& getVariable() const;


private:
	/**
	 * var function constructor
	 * @param variable initial value of variable
	**/
	VarFunction(const std::string& variable_);
public:

	static std::shared_ptr<Function> create(const std::string& variable_);

	/**
	 *  var function virtual destructor
	**/
	virtual ~VarFunction() {}

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

#endif /* __VAR_FUNCTION_H__ */

