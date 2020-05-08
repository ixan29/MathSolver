#ifndef __COS_FUNCTION_H__
#define __COS_FUNCTION_H__

#include <string>
#include <map>
#include <memory>

#include "Function.h"

/**
 * CosFunction class
 * 
 * TODO add description
**/
class CosFunction: public Function
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
	 * cos function constructor
	 * @param function initial value of function
	**/
	CosFunction(const std::shared_ptr<Function> function_);
public:
	static std::shared_ptr<Function> create(const std::shared_ptr<Function> function_);

	/**
	 *  cos function virtual destructor
	**/
	virtual ~CosFunction() {}

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

#endif /* __COS_FUNCTION_H__ */

