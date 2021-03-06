#include "../include/mal.hpp"

#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "../include/Environment.hpp"
#include "../include/Reader.hpp"
#include "../include/eval.hpp"
#include "../include/printer.hpp"
#include "../include/types.hpp"

mal::Data* READ(std::string input)
{
        return read_str(std::move(input));
}

// ast is not a list     :  then return the result of calling eval_ast on it
// ast is a empty list   :  return ast unchanged
// ast is a list, and first item is a Symbol        : then call the required eval function
// ast is a list, and first item is not a Symbol    : call eval apply
// all calls use the same env
mal::Data* EVAL(mal::Data* ast, mal::Environment& repl_env)
{
        if (ast->type() != mal::Data::Type::List)
                return eval::eval_ast(ast, repl_env);

        if (ast->list()->empty())
                return ast;

        auto* uneval_list = ast->list();
        switch (uneval_list->front()->type())
        {
                case mal::Data::Type::Symbol:
                {
                        auto* special_form = uneval_list->front()->symbol();

                        if (special_form->is_def())
                                return eval::eval_def(uneval_list, repl_env);

                        if (special_form->is_let())
                                return eval::eval_let(uneval_list, repl_env);

                        if (special_form->is_do())
                                return eval::eval_do(uneval_list, repl_env);

                        if (special_form->is_if())
                                return eval::eval_if(uneval_list, repl_env);

                        if (special_form->is_fn())
                                return eval::eval_fn(uneval_list, repl_env);
                }
                default:
                        return eval::eval_apply(ast, repl_env);
        }
}

std::string PRINT(mal::Data* result)
{
        return pr_str(result);
}

// returns an empty string if nothing is read
std::string rep(std::string input, mal::Environment& repl_env)
{
        if (auto* ast = READ(std::move(input)); ast)
        {
                auto* result = EVAL(ast, repl_env);
                auto  out    = PRINT(result);
                return out;
        }
        return "";
}
