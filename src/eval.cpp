#include "../include/eval.hpp"

#include <cassert>
#include <iostream>

#include "../include/Environment.hpp"
#include "../include/mal.hpp"
#include "../include/types.hpp"

namespace eval
{

static mal::Data* eval_let_list(mal::List* uneval_list, mal::Environment& new_env);

static mal::Data* eval_let_vec(mal::List* uneval_list, mal::Environment& new_env);

static mal::Data* eval_fn_list(mal::List* uneval_list, const mal::Environment& repl_env);

static mal::Data* eval_fn_vec(mal::List* uneval_list, const mal::Environment& repl_env);

mal::Data* eval_ast(mal::Data* ast, mal::Environment& repl_env)
{
        switch (ast->type())
        {
                case mal::Data::Type::Symbol:
                        return repl_env.lookup(ast->symbol());

                case mal::Data::Type::List:
                {
                        auto* eval_list = new mal::List;
                        for (auto& val : *ast->list())
                                eval_list->push(EVAL(val, repl_env));
                        return eval_list;
                }

                case mal::Data::Type::Vector:
                {
                        auto* eval_vec = new mal::Vector;
                        for (auto& val : *ast->vector())
                                eval_vec->push(EVAL(val, repl_env));
                        return eval_vec;
                }

                case mal::Data::Type::HashMap:
                {
                        auto* eval_map = new mal::HashMap;
                        for (auto& [key, val] : *ast->hashmap())
                                eval_map->insert(key, EVAL(val, repl_env));
                        return eval_map;
                }
                default:
                        return ast;
        }
}

mal::Data* eval_apply(mal::Data* ast, mal::Environment& repl_env)
{
        auto* args = eval::eval_ast(ast, repl_env)->list();
        if (args->front() && args->front()->type() == mal::Data::Type::Function)
        {
                auto* mal_fn = args->front()->function();
                return mal_fn->apply(args->size() - 1, args->data() + 1);
        }
        std::cerr << "EVAL apply - from EVAL default";
        return nullptr;
}

mal::Data* eval_def(mal::List* uneval_list, mal::Environment& repl_env)
{
        auto* sym_key = uneval_list->at(1);
        if (auto* mal_data = EVAL(uneval_list->at(2), repl_env); mal_data)
        {
                repl_env.set(sym_key->symbol(), mal_data);
                return mal_data;  // Prints def
        }
        std::cerr << "eval def";
        return nullptr;
}

mal::Data* eval_let(mal::List* uneval_list, mal::Environment& repl_env)
{
        auto* new_env = new mal::Environment(&repl_env);

        if (uneval_list->at(1)->type() == mal::Data::Type::List)
                return eval_let_list(uneval_list, *new_env);

        if (uneval_list->at(1)->type() == mal::Data::Type::Vector)
                return eval_let_vec(uneval_list, *new_env);

        std::cerr << "eval let";
        return nullptr;
}

mal::Data* eval_do(mal::List* uneval_list, mal::Environment& repl_env)
{
        mal::Data* result{};
        assert(uneval_list->size() > 1);
        for (size_t i = 1; i < uneval_list->size(); ++i)
                result = EVAL(uneval_list->at(i), repl_env);
        return result;
}

mal::Data* eval_if(mal::List* uneval_list, mal::Environment& repl_env)
{
        auto* condition = EVAL(uneval_list->at(1), repl_env);
        if (condition->is_truthy())
        {
                auto* true_expr = uneval_list->at(2);
                return EVAL(true_expr, repl_env);
        }

        auto* false_expr = (uneval_list->size() > 3) ? uneval_list->at(3)
                                                     : new mal::Nil;
        return EVAL(false_expr, repl_env);
}

mal::Data* eval_fn(mal::List* uneval_list, const mal::Environment& repl_env)
{
        if (uneval_list->at(1)->type() == mal::Data::Type::List)
                return eval_fn_list(uneval_list, repl_env);

        if (uneval_list->at(1)->type() == mal::Data::Type::Vector)
                return eval_fn_vec(uneval_list, repl_env);

        std::cerr << "eval fn";
        return nullptr;
}

static mal::Data* eval_let_list(mal::List* uneval_list, mal::Environment& new_env)
{
        auto* bindings = uneval_list->at(1)->list();
        for (size_t i = 0; i < bindings->size(); i += 2)
        {
                auto* sym_key = bindings->at(i);
                assert(i + 1 < bindings->size());
                if (auto* mal_data = EVAL(bindings->at(i + 1), new_env); mal_data)
                        new_env.set(sym_key->symbol(), mal_data);
        }
        return EVAL(uneval_list->at(2), new_env);
}

static mal::Data* eval_let_vec(mal::List* uneval_list, mal::Environment& new_env)
{
        auto* bindings = uneval_list->at(1)->vector();
        for (size_t i = 0; i < bindings->size(); i += 2)
        {
                auto* sym_key = bindings->at(i);
                assert(i + 1 < bindings->size());
                if (auto* mal_data = EVAL(bindings->at(i + 1), new_env); mal_data)
                        new_env.set(sym_key->symbol(), mal_data);
        }
        return EVAL(uneval_list->at(2), new_env);
}

static mal::Data* eval_fn_list(mal::List* uneval_list, const mal::Environment& repl_env)
{
        auto* binds   = uneval_list->at(1)->list();
        auto* fn_body = uneval_list->at(2);

        auto closure = [&repl_env, binds, fn_body](const std::size_t argc, mal::Data* const* args)
            -> mal::Data* {
                auto* exprs = new mal::List;
                for (size_t i = 0; i < argc; ++i)
                        exprs->push(args[i]);
                auto* fn_env = new mal::Environment(&repl_env, binds, exprs);
                return EVAL(fn_body, *fn_env);
        };
        return new mal::Function(closure);
}

static mal::Data* eval_fn_vec(mal::List* uneval_list, const mal::Environment& repl_env)
{
        auto* binds   = uneval_list->at(1)->vector();
        auto* fn_body = uneval_list->at(2);

        auto closure = [&repl_env, binds, fn_body](const std::size_t argc, mal::Data* const* args)
            -> mal::Data* {
                auto* exprs = new mal::List;
                for (size_t i = 0; i < argc; ++i)
                        exprs->push(args[i]);
                auto* fn_env = new mal::Environment(&repl_env, binds, exprs);
                return EVAL(fn_body, *fn_env);
        };
        return new mal::Function(closure);
}

}  // namespace eval
