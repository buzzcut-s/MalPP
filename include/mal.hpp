#ifndef MAL_HPP
#define MAL_HPP

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace mal
{
class Data;
class Symbol;
class Integer;
using DataPtr = std::unique_ptr<mal::Data>;
}  // namespace mal

// TODO(piyush) Should be mal::Data*?
using int_fn = std::function<mal::Integer*(const mal::Integer* lhs, const mal::Integer* rhs)>;
// TODO(piyush) Do I have to pass a hasher and predicate?
using repl_env = std::unordered_map<mal::Symbol*, int_fn>;

mal::DataPtr READ(std::string input);

mal::DataPtr EVAL(mal::DataPtr ast, const repl_env& env);

// TODO(piyush) Uh?
mal::DataPtr eval_ast(mal::DataPtr ast, const repl_env& env);

std::string PRINT(mal::DataPtr input);

std::string rep(std::string input);

#endif  // MAL_HPP
