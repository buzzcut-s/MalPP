#ifndef MAL_HPP
#define MAL_HPP

#include <memory>
#include <string>

namespace mal
{
class Data;
class Environment;
}  // namespace mal

mal::Data* READ(std::string input);

mal::Data* EVAL(mal::Data* ast, mal::Environment& repl_env);

std::string PRINT(mal::Data* result);

std::string rep(std::string input, mal::Environment& repl_env);

#endif  // MAL_HPP
