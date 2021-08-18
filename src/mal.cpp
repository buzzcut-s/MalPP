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

mal::Data* EVAL(mal::Data* ast, mal::Environment& repl_env)
{
        if (ast->type() != mal::Data::Type::List)
        {
                auto* ret = eval::eval_ast(ast, repl_env);
                return ret;
        }

        if (ast->list()->empty())
                return new mal::List();

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
                {
                        auto* args = eval::eval_ast(ast, repl_env)->list();
                        if (args->front() && args->front()->type() == mal::Data::Type::Function)
                        {
                                auto* mal_fn = args->front()->function();
                                auto* ret    = mal_fn->apply(args->size() - 1, args->data() + 1);
                                return ret;
                        }
                        std::cerr << "EVAL Default";
                        return nullptr;
                }
        }
}

std::string PRINT(mal::Data* result)
{
        return pr_str(result);
}

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
