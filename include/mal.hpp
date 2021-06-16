#ifndef MAL_HPP
#define MAL_HPP

#include <string>

#include "types.hpp"

mal::Data* READ(std::string_view input);

mal::Data* EVAL(mal::Data* value);

std::string PRINT(mal::Data* input);

std::string rep(std::string_view input);

#endif  // MAL_HPP
