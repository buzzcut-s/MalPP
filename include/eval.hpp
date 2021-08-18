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

mal::Data* eval_apply(mal::Data* ast, mal::Environment& repl_env);

mal::Data* eval_def(mal::List* uneval_list, mal::Environment& repl_env);

mal::Data* eval_let(mal::List* uneval_list, mal::Environment& repl_env);

mal::Data* eval_do(mal::List* uneval_list, mal::Environment& repl_env);

mal::Data* eval_if(mal::List* uneval_list, mal::Environment& repl_env);

mal::Data* eval_fn(mal::List* uneval_list, const mal::Environment& repl_env);

}  // namespace eval

#endif  // EVAL_HPP
