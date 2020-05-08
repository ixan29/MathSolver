#ifndef __MATRIX_FUNCTION_H__
#define __MATRIX_FUNCTION_H__

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Function.h"

/**
 * MatrixFunction class
 * 
 * TODO add description
**/
class MatrixFunction: public Function
{
private:
	std::vector<std::vector<std::shared_ptr<Function>>> functions;

	/**
	 * matrix function constructor
	 * @param functions initial value of functions
	**/
	MatrixFunction(size_t rows, size_t cols);
public:
	static std::shared_ptr<MatrixFunction> create(size_t rows, size_t cols);
	static std::shared_ptr<MatrixFunction> identity(size_t size);

	/**
	 *  matrix function virtual destructor
	**/
	virtual ~MatrixFunction() {}

	size_t getRows() const;
	size_t getCols() const;
	std::shared_ptr<Function> getFunction(size_t row, size_t col) const;
	void setFunction(size_t row, size_t col, std::shared_ptr<Function> function);

	std::shared_ptr<MatrixFunction> transpose() const;
	std::shared_ptr<Function> determinant() const;
	std::shared_ptr<MatrixFunction> inverse() const;

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

#endif /* __MATRIX_FUNCTION_H__ */

