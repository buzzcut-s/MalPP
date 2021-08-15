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
}  // namespace mal

// TODO(piyush) Should be mal::Data*?
using int_fn = std::function<mal::Integer*(const mal::Integer* lhs, const mal::Integer* rhs)>;
// TODO(piyush) Do I have to pass a hasher and predicate?
using repl_env = std::unordered_map<mal::Symbol*, int_fn>;

std::unique_ptr<mal::Data> READ(std::string input);

std::unique_ptr<mal::Data> EVAL(std::unique_ptr<mal::Data> ast, const repl_env& env);

// TODO(piyush) Uh?
std::unique_ptr<mal::Data> eval_ast(std::unique_ptr<mal::Data> ast, const repl_env& env);

std::string PRINT(std::unique_ptr<mal::Data> input);

std::string rep(std::string input);

#endif  // MAL_HPP
