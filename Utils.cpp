#include "Utils.h"

#include "ConstFunction.h"
#include "VarFunction.h"
#include "SumFunction.h"
#include "MulFunction.h"
#include "DivFunction.h"
#include "PowFunction.h"
#include "LogFunction.h"
#include "SinFunction.h"
#include "CosFunction.h"
#include "TanFunction.h"
#include "MatrixFunction.h"

#include <cstring>
#include <stdexcept>

bool needsParentheses(const Function& function)
{
	return function.getType() == typeid(SumFunction).hash_code();
}

std::shared_ptr<Function> createSerie(const std::shared_ptr<Function> function, const std::string& variable, const double value, const unsigned degree)
{
    std::map<std::string, std::shared_ptr<Function>> var {
        {variable, ConstFunction::create(value)}
    };
    
    std::vector<std::shared_ptr<Function>> terms;
    terms.reserve(degree+1);

    double factorial = 1;

    terms.push_back (
            function->solve(var)
    );

    std::shared_ptr<Function> der = function;

    for(unsigned i=1 ; i<=degree; i++)
    {
        der = der->derivative(variable);
        factorial *= i;

        terms.push_back(
            MulFunction::create({
                DivFunction::create (
                    der->solve(var),
                    ConstFunction::create(factorial)
                ),
                PowFunction::create (
                    SumFunction::create ({
                        VarFunction::create(variable),
                        ConstFunction::create(-value)
                    }),
                    ConstFunction::create(i)
                ) 
            })
        );
    }

    return SumFunction::create(terms);
}

std::vector<std::string> splitCommas(const std::string& str)
{
    std::vector<std::string> split {""};

    int countPar=0;

    for(char c : str)
        switch(c)
        {
            case '{':
            case '(': countPar++; break;

            case '}':
            case ')': countPar--; break;

            case ',':
                if(countPar==0)
                    split.push_back("");
                break;
            default: split.back() += c; break;
        }

    return split;
}

std::vector<std::string> splitSums(const std::string& str)
{
    std::vector<std::string> split {""};

    int countPar=0;

    for(char c : str)
        switch(c)
        {
            case '(': countPar++; break;
            case ')': countPar--; break;

            case '+':
                if(countPar==0)
                    split.push_back("");
                break;

            case '-':
                if(countPar==0)
                {
                    if(split.back()=="")
                        split.back() += '-';
                    else
                        split.push_back("-");            
                }
                break;
            
            default: split.back() += c; break;
        }

    return split;
}

std::vector<std::string> splitMuls(const std::string& str)
{
    std::vector<std::string> split {""};

    int countPar=0;

    for(char c : str)
        switch(c)
        {
            case '(': countPar++; break;
            case ')': countPar--; break;

            case '*':
                if(countPar==0)
                    split.push_back("");
                break;
            default: split.back() += c; break;
        }

    return split;
}

std::vector<std::string> splitDivs(const std::string& str)
{
    std::vector<std::string> split {""};

    int countPar=0;

    for(char c : str)
        switch(c)
        {
            case '(': countPar++; break;
            case ')': countPar--; break;

            case '/':
                if(countPar==0)
                    split.push_back("");
                break;
            default: split.back() += c; break;
        }

    return split;
}

std::vector<std::string> splitPow(const std::string& str)
{
    std::vector<std::string> split {""};

    int countPar=0;

    for(char c : str)
        switch(c)
        {
            case '(': countPar++; break;
            case ')': countPar--; break;

            case '^':
                if(countPar==0)
                    split.push_back("");
                break;
            default: split.back() += c; break;
        }

    return split;
}

bool isNumber(const std::string& str)
{
    for(char c : str)
        if(!isdigit(c)
        && c != '.'
        && c != '-'
        )
            return false;
    
    return true;
}

std::shared_ptr<Function> parse(const std::string& expr, std::map<std::string, std::shared_ptr<Function>>& map)
{
    std::string str = expr;
    
    auto sums = splitSums(str);

    if(sums.size() > 1)
    {
        std::vector<std::shared_ptr<Function>> s;
        s.reserve(sums.size());

        for(auto& sum : sums)
            s.push_back(parse(sum, map));

        return SumFunction::create(s);
    }

    auto muls = splitMuls(str);

    if(muls.size() > 1)
    {
        std::vector<std::shared_ptr<Function>> m;
        m.reserve(muls.size());

        for(auto& mul : muls)
            m.push_back(parse(mul, map));

        return MulFunction::create(m);
    }

    auto divs = splitDivs(str);

    if(divs.size() > 1)
    {
        std::vector<std::shared_ptr<Function>> d;
        d.reserve(divs.size());

        for(auto& div : divs)
            d.push_back(parse(div, map));

        return
        DivFunction::create(
            d[0],
            MulFunction::create(
                std::vector<std::shared_ptr<Function>>(d.begin()+1, d.end())
            )
        );
    }

    auto pows = splitPow(str);

    if(pows.size() > 2)
        throw std::runtime_error("powers are anbiguous here: "+str+". Use parentheses plz");

    if(pows.size() == 2)
        return
        PowFunction::create (
            parse(pows[0], map),
            parse(pows[1], map)
        );

    if(str[0]=='(')
        if(str.back()==')')
            return parse(str.substr(1, str.length()-2), map);
        else
            throw std::runtime_error("missing closing bracket after "+str);

    if(str.find("log")==0)
        return LogFunction::create(parse(str.substr(3), map));

    if(str.find("sin")==0)
        return SinFunction::create(parse(str.substr(3), map));

    if(str.find("cos")==0)
        return CosFunction::create(parse(str.substr(3), map));

    if(str.find("tan")==0)
        return TanFunction::create(parse(str.substr(3), map));

    if(str.find("matrix")==0)
    {
        auto numsStr = splitCommas(str.substr(7, str.length()-8));

        return MatrixFunction::create(
            std::stoi(numsStr[0]),
            std::stoi(numsStr[1])
        );
    }

    if(str.find("identity")==0)
    {
        auto numStr = str.substr(9, str.length()-10);
        return MatrixFunction::identity(std::stoi(numStr));
    }

    if(str.find("transpose")==0)
    {
        auto func = parse(str.substr(10, str.length()-11), map);

        if(func->getType() == typeid(MatrixFunction).hash_code())
            return std::static_pointer_cast<MatrixFunction>(func)->transpose();

        throw std::runtime_error("Getting a transpose from a non-matrix function is impossible");
    }

    if(str.find("det")==0)
    {
        auto func = parse(str.substr(4, str.length()-5), map);

        if(func->getType() == typeid(MatrixFunction).hash_code())
            return std::static_pointer_cast<MatrixFunction>(func)->determinant();

        throw std::runtime_error("Getting the determinant from a non-matrix function is impossible");
    }

    if(str.find("derivative{")==0)
    {
        if(str.back() != '}')
            throw std::runtime_error("missing closing bracket a the end of "+str);

        str = str.substr(strlen("derivative{"));
        str = str.substr(0, str.length()-1);

        return
        parse(str.substr(str.find(',')+1), map)
        ->derivative(str.substr(0, str.find(",")));
    }

    if(str.find('(') < str.length())
    {
        if(str.back() != ')')
            throw std::runtime_error("missing parenthese at the end of "+str);

        std::string variable = str.substr(0, str.find('('));

        str = str.substr(str.find('(')+1);
        str = str.substr(0, str.length()-1);

        std::map<std::string, std::shared_ptr<Function>> localFunctions;

        for(auto arg : splitCommas(str))
        {
            if(arg.find('=') >= arg.length())
                throw std::runtime_error("missing = symbol at argument: "+arg);

            localFunctions[arg.substr(0, arg.find('='))] =
            parse(arg.substr(arg.find('=')+1), map);
        }

        return map[variable]->solve(localFunctions); 
    }

    if(isNumber(str))
        return ConstFunction::create(std::stod(str));
    else
    if(map.count(str))
        return map[str];
    else
        return VarFunction::create(str);
}