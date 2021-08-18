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

mal::DataPtr READ(std::string input)
{
        return read_str(std::move(input));
}

mal::Data* EVAL(mal::Data* ast, mal::Environment& repl_env)
{
        using mal::Data::AllocType::Nude;

        if (ast->type() != mal::Data::Type::List)
                return eval::eval_ast(ast, repl_env);

        if (ast->list()->empty())
                return new mal::List(Nude);

        auto* uneval_list = ast->list();
        switch (uneval_list->front()->type())
        {
                case mal::Data::Type::Symbol:
                {
                        auto* first = uneval_list->front()->symbol();
                        if (first->is_def())
                                return eval::eval_def(uneval_list, repl_env);

                        if (first->is_let())
                                return eval::eval_let(uneval_list, repl_env);
                }
                default:
                {
                        auto* args = eval::eval_ast(ast, repl_env)->eval_list();
                        if (args->front() && args->front()->type() == mal::Data::Type::Function)
                        {
                                auto* mal_fn = args->front()->function();
                                auto* ret    = mal_fn->apply(args->size() - 1, args->data() + 1);
                                delete args;
                                return ret;
                        }
                        delete args;
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
        if (auto ast = READ(std::move(input)); ast)
        {
                auto* result = EVAL(ast.get(), repl_env);
                auto  out    = PRINT(result);

                if (result && result->alloc_type() == mal::Data::AllocType::Nude)
                        delete result;

                return out;
        }
        return "";
}
