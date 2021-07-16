#ifndef MAL_HPP
#define MAL_HPP

#include <functional>
#include <string>
#include <unordered_map>

namespace mal
{
class Data;
class Symbol;
class Integer;
}  // namespace mal

// TODO(piyush) Should be mal::Data*?
using int_fn = std::function<mal::Integer*(const mal::Integer* lhs, const mal::Integer* rhs)>;
// TODO(piyush) Do I have to pass a hasher and predicate?
using repl_env = std::unordered_map<mal::Symbol*, int_fn>;

mal::Data* READ(std::string input);

mal::Data* EVAL(mal::Data* ast, const repl_env& env);

// TODO(piyush) Uh?
mal::Data* eval_ast(mal::Data* ast, const repl_env& env);

std::string PRINT(mal::Data* input);

std::string rep(std::string input);

#endif  // MAL_HPP
