#ifndef MAL_HPP
#define MAL_HPP

#include <string>

const std::string& READ(const std::string& input);

const std::string& EVAL(const std::string& ast);

const std::string& PRINT(const std::string& ast);

const std::string& rep(const std::string& input);

#endif  // MAL_HPP
