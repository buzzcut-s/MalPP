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

mal::Data* EVAL(mal::Data* ast, const mal::Environment& repl_env);

mal::Data* eval_ast(mal::Data* ast, const mal::Environment& repl_env);

std::string PRINT(mal::Data* result);

std::string rep(std::string input, const mal::Environment& repl_env);

#endif  // MAL_HPP
