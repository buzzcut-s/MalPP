#ifndef MAL_HPP
#define MAL_HPP

#include <memory>
#include <string>

namespace mal
{
class Data;
class Environment;
using DataPtr = std::unique_ptr<mal::Data>;
}  // namespace mal

mal::DataPtr READ(std::string input);

mal::Data* EVAL(mal::Data* ast, const mal::Environment& env);

mal::Data* eval_ast(mal::Data* ast, const mal::Environment& env);

std::string PRINT(mal::Data* input);

std::string rep(std::string input);

#endif  // MAL_HPP
