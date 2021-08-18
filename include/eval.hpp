#ifndef EVAL_HPP
#define EVAL_HPP

namespace mal
{
class Data;
class List;
class Environment;
}  // namespace mal

namespace eval
{

mal::Data* eval_ast(mal::Data* ast, mal::Environment& repl_env);

mal::Data* eval_def(mal::List* uneval_list, mal::Environment& repl_env);

mal::Data* eval_let(mal::List* uneval_list, mal::Environment& repl_env);

}  // namespace eval

#endif  // EVAL_HPP
