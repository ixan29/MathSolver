
#ifndef __UTILS_H__
#define __UTILS_H__

#include "Function.h"

bool needsParentheses(const Function& function);

std::shared_ptr<Function> createSerie(const std::shared_ptr<Function> function, const std::string& variable, const double value, const unsigned degree);
std::shared_ptr<Function> parse(const std::string& str, std::map<std::string, std::shared_ptr<Function>>& map);

#endif /* __UTILS_H__ */