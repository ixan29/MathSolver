#include "ConstFunction.cpp"
#include "VarFunction.cpp"
#include "SumFunction.cpp"
#include "MulFunction.cpp"
#include "DivFunction.cpp"
#include "LogFunction.cpp"
#include "PowFunction.cpp"
#include "SinFunction.cpp"
#include "CosFunction.cpp"
#include "TanFunction.cpp"
#include "TrigFunctionsUtils.cpp"
#include "MatrixFunction.cpp"
#include "Utils.cpp"

#include <iostream>

using namespace std;

int main()
{
/*
    auto func =
    DivFunction::create(
        SumFunction::create({
            PowFunction::create(
                VarFunction::create("x"),
                ConstFunction::create(2)
            ),
            VarFunction::create("x")
        }),
        PowFunction::create (
            VarFunction::create("x"),
            ConstFunction::create(2)
        )
    );

    std::cout << "fin" << std::endl;
    std::cout << *func << std::endl;
*/
/*
    auto matrix = MatrixFunction::create(2,2);

    matrix->setFunction(0, 0, VarFunction::create("a"));
    matrix->setFunction(0, 1, VarFunction::create("b"));
    matrix->setFunction(1, 0, VarFunction::create("c"));
    matrix->setFunction(1, 1, VarFunction::create("d"));

    auto inv = matrix->inverse();
    auto det = matrix->determinant();

    std::cout << "det" << std::endl;
    std::cout << *det << std::endl;

    std::cout << "inv";
    std::cout << *inv;

    for(size_t row=0 ; row<2 ; row++)
		for(size_t col=0 ; col<2 ; col++)
        {
            auto mul =
            MulFunction::create({
                inv->getFunction(row, col),
                det
            });

            std::cout << *mul << std::endl;
        }

    return 0;
*/
    map<string, shared_ptr<Function>> functions;

    do
    {
        try
        {
            cout << endl << ">>";

            string expr;
            getline(cin, expr);

            string str;
            for(char c : expr)
                if(c != ' ')
                    str += c;
                    
            if(str == "exit")
                return 0;
            else
            if(str.find('=') < str.find('('))
            {
                string variable = str.substr(0, str.find('='));

                if(variable.find('[') < variable.length())
                {
                    string numData = variable.substr(variable.find('[')+1);
                    numData = numData.substr(0, numData.length()-1);
                    variable = variable.substr(0, variable.find('['));

                    if(functions[variable]->getType() != typeid(MatrixFunction).hash_code())
                        throw std::runtime_error("cannot apply indexers to a non-matrix function");

                    auto nums = splitCommas(numData);

                    auto matrix = std::static_pointer_cast<MatrixFunction>(functions[variable]);

                    matrix->
                    setFunction (
                        std::stoi(nums[0]),
                        std::stoi(nums[1]),
                        parse(str.substr(str.find('=')+1), functions)
                    );
                }
                else
                {
                    functions[variable] = parse(str.substr(str.find('=')+1), functions);   
                }
            }
            else
            {
                auto f = parse(str, functions);

                cout << endl <<
                "ans =" << endl <<
                '\t' << *f << endl;
            }
            
        }
        catch(exception& e)
        {
            cout << e.what() << endl;
        }
    }
    while(true);

    return 0;
}