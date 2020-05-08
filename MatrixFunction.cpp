#include "ConstFunction.h"
#include "MatrixFunction.h"
#include "SumFunction.h"
#include "MulFunction.h"
#include "DivFunction.h"

#include "sstream"
#include "cmath"

MatrixFunction::MatrixFunction(size_t rows, size_t cols)
:Function(typeid(MatrixFunction).hash_code())
,functions(rows)
{
	for(auto& row : functions)
		row.resize(cols);
}

std::shared_ptr<MatrixFunction> MatrixFunction::create(size_t rows, size_t cols) {
	std::shared_ptr<MatrixFunction> matrix (new MatrixFunction(rows, cols));

	for(size_t row=0 ; row<rows ; row++)
		for(size_t col=0 ; col<cols ; col++)
			matrix->setFunction(row, col, ConstFunction::create(0));

	return matrix;
}

std::shared_ptr<MatrixFunction> MatrixFunction::identity(size_t size)
{
	std::shared_ptr<MatrixFunction> matrix (new MatrixFunction(size, size));

	for(size_t row=0 ; row<size ; row++)
		for(size_t col=0 ; col<size ; col++)
			matrix->setFunction(row, col, ConstFunction::create(row==col));

	return matrix;
}

size_t MatrixFunction::getRows() const {
	return functions.size();
}

size_t MatrixFunction::getCols() const {
	return 	functions.size() == 0
	?		0
	:		functions[0].size();
}

std::shared_ptr<Function> MatrixFunction::getFunction(size_t row, size_t col) const {
	if(row >= getRows() || col >= getCols())
		throw std::runtime_error("Access to matrix function out of bounds");

	return functions[row][col];
}

void MatrixFunction::setFunction(size_t row, size_t col, std::shared_ptr<Function> function) {
	if(row >= getRows() || col >= getCols())
		throw std::runtime_error("Modification of matrix function out of bounds");

	functions[row][col] = function;
}

std::shared_ptr<MatrixFunction> MatrixFunction::transpose() const {
	std::shared_ptr<MatrixFunction> transpose (new MatrixFunction(getCols(), getRows()));

	for(size_t row=0 ; row<getRows() ; row++)
		for(size_t col=0 ; col<getCols() ; col++)
			transpose->functions[col][row] = functions[row][col];

	return transpose;
}

std::shared_ptr<Function> MatrixFunction::determinant() const
{
	if(getRows() != getCols())
		throw std::runtime_error("Getting the determinant from a non-square matrix is forbidden");

	//create a copy of the current matrix since the method is declared constant
	std::shared_ptr<MatrixFunction> matrix ( new MatrixFunction(getRows(), getCols()));

	for(size_t row=0 ; row < getRows(); row++)
		for(size_t col=0 ; col < getCols() ; col++)
			matrix->functions[row][col] = functions[row][col];

	//Lower triangle decomposition
	for(size_t level=0; level < getCols() ; level++)
	{
		for(size_t row=level+1 ; row < getRows() ; row++)
		{
			if(*matrix->functions[level][level] == *ConstFunction::create(0))
				return ConstFunction::create(0);

			auto mul =
			DivFunction::create(
				MulFunction::create ({
					ConstFunction::create(-1),
					matrix->functions[row][level]
				}),
				matrix->functions[level][level]
			);

			for(size_t col = level; col < getCols() ; col++)
			{
				matrix->functions[row][col] = 
				SumFunction::create({
					matrix->functions[row][col],
					MulFunction::create({
						matrix->functions[level][col],
						mul
					})
				});
			}
		}
	}

	//the determinant of a triangle matrix is the product of its diagonal elements;
	std::vector<std::shared_ptr<Function>> diagonals (getRows());

	for(size_t level=0 ; level<getRows() ; level++)
		diagonals[level] = matrix->functions[level][level];

	return MulFunction::create(diagonals);
}

//The matrix inverse is calculated using the Gauss-Jordan Method
std::shared_ptr<MatrixFunction> MatrixFunction::inverse() const
{
	if(getRows() != getCols())
		throw std::runtime_error("Getting the inverse from a non-square matrix is forbidden");

	//create a copy of the current matrix since the method is declared constant
	std::shared_ptr<MatrixFunction> matrix ( new MatrixFunction(getRows(), getCols()));

	for(size_t row=0 ; row < getRows(); row++)
		for(size_t col=0 ; col < getCols() ; col++)
			matrix->functions[row][col] = functions[row][col];

	//The inverse is initially an identity matrix in the Gauss Jordan elimination
	auto inverse = identity(getRows());

	//Lower triangle decomposition
	for(size_t level=0; level < getCols() ; level++)
	{
		for(size_t row=level+1 ; row < getRows() ; row++)
		{
			if(*matrix->functions[level][level] == *ConstFunction::create(0))
				throw std::runtime_error("The matrix is square but not invertible");

			auto mul =
			DivFunction::create(
				MulFunction::create ({
					ConstFunction::create(-1),
					matrix->functions[row][level]
				}),
				matrix->functions[level][level]
			);

			for(size_t col = level; col < getCols() ; col++)
			{
				matrix->functions[row][col] = 
				SumFunction::create({
					matrix->functions[row][col],
					MulFunction::create({
						matrix->functions[level][col],
						mul
					})
				});

				inverse->functions[row][col] = 
				SumFunction::create({
					inverse->functions[row][col],
					MulFunction::create({
						inverse->functions[level][col],
						mul
					})
				});
			}
		}
	}

	//Upper triange decomposition
	for(int level=getCols()-1; level>=0 ; level--)
	{
		for(int row=level-1 ; row>=0 ; row--)
		{
			if(*matrix->functions[level][level] == *ConstFunction::create(0))
				throw std::runtime_error("The matrix is square but not invertible");

			auto mul = DivFunction::create(
				MulFunction::create ({
					ConstFunction::create(-1),
					matrix->functions[row][level]
				}),
				matrix->functions[level][level]
			);

			for(int col = level; col >= 0 ; col--)
			{
				matrix->functions[row][col] = 
				SumFunction::create({
					matrix->functions[row][col],
					MulFunction::create({
						matrix->functions[level][col],
						mul
					})
				});

				inverse->functions[row][col] = 
				SumFunction::create({
					inverse->functions[row][col],
					MulFunction::create({
						inverse->functions[level][col],
						mul
					})
				});
			}
		}
	}

	//Diagonal decomposition
	for(size_t level=0; level < getCols() ; level++)
	{
		for(size_t col = 0; col < getCols() ; col++)
		{
			if(*matrix->functions[level][level] == *ConstFunction::create(0))
				throw std::runtime_error("The matrix is square but not invertible");

			inverse->functions[level][col] = 
			DivFunction::create(
				inverse->functions[level][col],
				matrix->functions[level][level]
			);
		}
	}

	return inverse;
}

std::shared_ptr<Function> MatrixFunction::solve(std::map<std::string,std::shared_ptr<Function>>& variables)
{
	std::shared_ptr<MatrixFunction> solved (new MatrixFunction(getRows(), getCols()));

	for(size_t row=0; row<getRows(); row++)
		for(size_t col=0 ; col<getCols(); col++)
			solved->functions[row][col] = functions[row][col]->solve(variables);

	return solved;
}

std::shared_ptr<Function> MatrixFunction::derivative(std::string variable)
{
	std::shared_ptr<MatrixFunction> derivative (new MatrixFunction(getRows(), getCols()));

	for(size_t row=0; row<getRows(); row++)
		for(size_t col=0 ; col<getCols(); col++)
			derivative->functions[row][col] = functions[row][col]->derivative(variable);

	return derivative;
}

void MatrixFunction::print(std::ostream& os) const
{
	std::vector<std::vector<std::string>> strs (getRows());

	for(auto& row : strs)
		row.resize(getCols());

	for(size_t row=0; row<getRows(); row++)
		for(size_t col=0 ; col<getCols(); col++)
		{
			std::ostringstream oss;
			oss << *functions[row][col];
			strs[row][col] = oss.str();
		}

	size_t max_length = 1;

	for(size_t row=0; row<getRows(); row++)
		for(size_t col=0 ; col<getCols(); col++)
			max_length = std::max(max_length, strs[row][col].length());

	os << std::endl << "[" << std::endl;

	for(size_t row=0; row<getRows(); row++)
	{
		os << "\t[" << strs[row][0] << std::string(max_length-strs[row][0].length(), ' ');

		for(size_t col=1 ; col<getCols(); col++)
			os << ',' << strs[row][col] << std::string(max_length-strs[row][col].length(), ' ');

		os << "]" << std::endl;
	}

	os << "]" << std::endl;
}
	
bool MatrixFunction::equals(const Function& other) const
{
	if(other.getType() != typeid(MatrixFunction).hash_code())
		return false;

	const MatrixFunction& matOther = static_cast<const MatrixFunction&>(other);

	if(getRows() != matOther.getRows()
	|| getCols() != matOther.getCols())
		return false;

	for(size_t row=0; row<getRows(); row++)
		for(size_t col=0 ; col<getCols(); col++)
			if(*functions[row][col] != *matOther.functions[row][col])
				return false;

	return true;
}
