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

        auto* eval_list = eval_ast(ast, env)->eval_list();
        auto* fn        = eval_list->at(0)->function();

        return fn->apply(eval_list->size() - 1, eval_list->data() + 1);
}

mal::Data* eval_ast(mal::Data* ast, const mal::Environment& env)
{
        if (ast->type() == mal::Data::Type::Symbol)
        {
                if (auto fn = env.lookup(ast); fn)
                {
                        return fn.value();
                }
                std::cerr << "Symbol not found : " << ast->symbol()->format() << std::endl;
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
        auto env = mal::Environment{};

        auto  ast    = READ(std::move(input));
        auto* result = EVAL(ast.get(), env);

        return PRINT(result);
}
