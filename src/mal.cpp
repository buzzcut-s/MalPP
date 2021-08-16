#include "../include/mal.hpp"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "../include/Environment.hpp"
#include "../include/Reader.hpp"
#include "../include/printer.hpp"
#include "../include/types.hpp"

mal::DataPtr READ(std::string input)
{
        return read_str(std::move(input));
}

mal::Data* EVAL(mal::Data* ast, const mal::Environment& env)
{
        if (ast->type() != mal::Data::Type::List)
                return eval_ast(ast, env);

        if (ast->list()->empty())
                return ast;

        auto* args = eval_ast(ast, env)->eval_list();
        if (args->at(0))
        {
                auto* fn  = args->at(0)->function();
                auto* ret = fn->apply(args->size() - 1, args->data() + 1);
                delete args;
                return ret;
        }

        return nullptr;
}

mal::Data* eval_ast(mal::Data* ast, const mal::Environment& env)
{
        if (ast->type() == mal::Data::Type::Symbol)
        {
                if (auto fn = env.lookup(ast); fn)
                {
                        return fn.value();
                }
                return nullptr;
        }

        if (ast->type() == mal::Data::Type::List)
        {
                auto eval_list = new mal::EvalList();
                for (auto& val : *ast->list())
                {
                        eval_list->push(EVAL(val.get(), env));
                }
                return eval_list;
        }

        return ast;
}

std::string PRINT(mal::Data* input)
{
        return pr_str(input);
}

std::string rep(std::string input)
{
        static auto env = mal::Environment{};
        if (auto ast = READ(std::move(input)); ast)
        {
                auto* result = EVAL(ast.get(), env);
                auto  ret    = PRINT(result);
                delete result;
                return ret;
        }
        return "";
}
