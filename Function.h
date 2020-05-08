#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <string>
#include <map>
#include <memory>
#include <ostream>

/**
 * Function interface
 * 
 * TODO add description
**/
class Function
{
private:
	std::size_t type;

public: 
	/**
	 * Gets the type.
	 * @return the type
	**/
	const std::size_t& getType() const {
		return type;
	}

	Function(std::size_t type_)
	:type(type_)
	{}

	/**
	 *  function virtual destructor
	**/
	virtual ~Function() {}

	/**
	 * TODO complete method documentation
	 * @param variables
	 * @return
	**/
	virtual std::shared_ptr<Function> solve(std::map<std::string,std::shared_ptr<Function>>& variables) = 0;
	/**
	 * TODO complete method documentation
	 * @param variable
	 * @return
	**/
	virtual std::shared_ptr<Function> derivative(std::string variable) = 0;

protected:
	virtual void print(std::ostream& os) const = 0;
	virtual bool equals(const Function& other) const = 0;

public:
	friend std::ostream& operator<<(std::ostream& os, const Function& function);
	friend bool operator==(const Function& a, const Function& b);
};

std::ostream& operator<<(std::ostream& os, const Function& function) {
	function.print(os);
	return os;
}

bool operator==(const Function& a, const Function& b) {
	return a.equals(b);
}

bool operator!=(const Function& a, const Function& b) {
	return ! ( a == b );
}

#endif /* __FUNCTION_H__ */

