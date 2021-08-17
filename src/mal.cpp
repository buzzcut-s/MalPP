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

mal::Data* EVAL(mal::Data* ast, const mal::Environment& repl_env)
{
        using mal::Data::AllocType::Nude;

        if (ast->type() != mal::Data::Type::List)
                return eval_ast(ast, repl_env);

        if (ast->list()->empty())
                return new mal::List(Nude);

        auto* args = eval_ast(ast, repl_env)->eval_list();
        if (args->at(0) && args->at(0)->type() == mal::Data::Type::Function)
        {
                auto* mal_fn = args->at(0)->function();
                auto* ret    = mal_fn->apply(args->size() - 1, args->data() + 1);
                delete args;
                return ret;
        }
        delete args;
        return nullptr;
}

mal::Data* eval_ast(mal::Data* ast, const mal::Environment& repl_env)
{
        using mal::Data::AllocType::Nude;

        switch (ast->type())
        {
                case mal::Data::Type::Symbol:
                {
                        if (auto fn = repl_env.lookup(ast); fn)
                        {
                                return fn.value();
                        }
                        // TODO(piyush) Handle an exception here
                        std::cerr << "'" << ast->format() << "' not found";
                        return nullptr;
                }

                case mal::Data::Type::List:
                {
                        auto* eval_list = new mal::EvalList(Nude);
                        for (auto& val : *ast->list())
                        {
                                eval_list->push(EVAL(val.get(), repl_env));
                        }
                        return eval_list;
                }

                case mal::Data::Type::Vector:
                {
                        auto* eval_vec = new mal::EvalVector(Nude);
                        for (auto& val : *ast->vector())
                        {
                                eval_vec->push(EVAL(val.get(), repl_env));
                        }
                        return eval_vec;
                }

                case mal::Data::Type::HashMap:
                {
                        auto* eval_map = new mal::EvalHashMap(Nude);
                        for (auto& [key, val] : *ast->hashmap())
                        {
                                eval_map->insert(key.get(), EVAL(val.get(), repl_env));
                        }
                        return eval_map;
                }

                default:
                        return ast;
        }
}

std::string PRINT(mal::Data* result)
{
        return pr_str(result);
}

std::string rep(std::string input, const mal::Environment& repl_env)
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
