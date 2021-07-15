#ifndef MAL_HPP
#define MAL_HPP

#include <string>

namespace mal
{
class Data;
}

mal::Data* READ(std::string input);

mal::Data* EVAL(mal::Data* value);

std::string PRINT(mal::Data* input);

std::string rep(std::string input);

#endif  // MAL_HPP
