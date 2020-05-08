#ifndef __LOG_FUNCTION_H__
#define __LOG_FUNCTION_H__

#include <string>
#include <map>
#include <memory>

#include "Function.h"

/**
 * LogFunction class
 * 
 * TODO add description
**/
class LogFunction: public Function
{
	std::shared_ptr<Function> function;

public:
	std::shared_ptr<Function> getFunction() const;

private:
	/**
	 * log function constructor
	 * @param function initial value of function
	**/
	LogFunction(const std::shared_ptr<Function> function_);
public:
	/**
	 *  log function virtual destructor
	**/
	virtual ~LogFunction() {}

	static std::shared_ptr<Function> create(const std::shared_ptr<Function> function_);
	static std::shared_ptr<Function> create(const std::shared_ptr<Function> log, const std::shared_ptr<Function> function_);

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

#endif /* __LOG_FUNCTION_H__ */

