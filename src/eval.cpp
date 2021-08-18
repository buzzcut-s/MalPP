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

static mal::Data* clone(mal::Data* mal_data);

mal::Data* eval_ast(mal::Data* ast, mal::Environment& repl_env)
{
        using mal::Data::AllocType::Nude;

        switch (ast->type())
        {
                case mal::Data::Type::Symbol:
                {
                        return repl_env.lookup(ast->symbol());
                }

                case mal::Data::Type::List:
                {
                        auto* eval_list = new mal::EvalList(Nude);
                        for (auto& val : *ast->list())
                                eval_list->push(EVAL(val.get(), repl_env));
                        return eval_list;
                }

                case mal::Data::Type::Vector:
                {
                        auto* eval_vec = new mal::EvalVector(Nude);
                        for (auto& val : *ast->vector())
                                eval_vec->push(EVAL(val.get(), repl_env));
                        return eval_vec;
                }

                case mal::Data::Type::HashMap:
                {
                        auto* eval_map = new mal::EvalHashMap(Nude);
                        for (auto& [key, val] : *ast->hashmap())
                                eval_map->insert(key.get(), EVAL(val.get(), repl_env));
                        return eval_map;
                }
                default:
                        return ast;
        }
}

mal::Data* eval_def(mal::List* uneval_list, mal::Environment& repl_env)
{
        auto* sym_key = uneval_list->at(1);
        if (auto* mal_data = EVAL(uneval_list->at(2), repl_env); mal_data)
        {
                repl_env.set(clone(sym_key)->symbol(), clone(mal_data));
                return mal_data;  // Prints def
        }
        std::cerr << "eval def";
        return nullptr;
}

mal::Data* eval_let(mal::List* uneval_list, mal::Environment& repl_env)
{
        auto* new_env = new mal::Environment(&repl_env);
        repl_env.set_inner(new_env);

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
                result = eval::eval_ast(uneval_list->at(i), repl_env);
        return result;
}

mal::Data* eval_if(mal::List* uneval_list, mal::Environment& repl_env)
{
        auto* condition  = uneval_list->at(1);
        auto* true_expr  = uneval_list->at(2);
        auto* false_expr = (uneval_list->size() > 3) ? uneval_list->at(3)
                                                     : new mal::Nil(mal::Data::AllocType::Nude);
        if (EVAL(condition, repl_env)->is_truthy())
                return EVAL(true_expr, repl_env);
        return EVAL(false_expr, repl_env);
}

mal::Data* eval_fn(mal::List* uneval_list, mal::Environment& repl_env)
{
        auto* env_ptr = &repl_env;
        auto* binds   = uneval_list->at(1)->list();
        auto* body    = uneval_list->at(2);
        auto  closure = [env_ptr, binds, body](const std::size_t argc,
                                              mal::Data* const* args)
            -> mal::Data* {
                auto* exprs = new mal::FnList(mal::Data::AllocType::Nude);
                for (size_t i = 0; i < argc; ++i)
                        exprs->push(args[i]);
                auto* fn_env = new mal::Environment(env_ptr, binds, exprs);
                return EVAL(body, *fn_env);
        };
        return new mal::Function(closure, mal::Data::AllocType::Nude);
}

static mal::Data* eval_let_list(mal::List* uneval_list, mal::Environment& new_env)
{
        auto* bindings = uneval_list->at(1)->list();
        for (size_t i = 0; i < bindings->size(); i += 2)
        {
                auto* sym_key = bindings->at(i);
                assert(i + 1 < bindings->size());
                if (auto* mal_data = EVAL(bindings->at(i + 1), new_env); mal_data)
                {
                        new_env.set(clone(sym_key)->symbol(), clone(mal_data));
                        if (mal_data->alloc_type() == mal::Data::AllocType::Nude)
                                delete mal_data;
                }
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
                {
                        new_env.set(clone(sym_key)->symbol(), clone(mal_data));
                        if (mal_data->alloc_type() == mal::Data::AllocType::Nude)
                                delete mal_data;
                }
        }
        return EVAL(uneval_list->at(2), new_env);
}

static mal::Data* clone(mal::Data* mal_data)
{
        // TODO(piyush) This leaks memory, which by all means, it shouldn't.
        using mal::Data::AllocType::Clone;
        switch (mal_data->type())
        {
                case mal::Data::Type::Symbol:  // Leak
                        return new mal::Symbol(mal_data->symbol()->value(), Clone);
                case mal::Data::Type::Integer:  // Leak
                        return new mal::Integer(mal_data->integer()->value(), Clone);
                case mal::Data::Type::List:  // This doesn't leak
                {
                        auto* clone_list = new mal::CloneList(Clone);
                        for (auto& ptr : *mal_data->list())
                        {
                                clone_list->push(clone(ptr.get()));
                        }
                        return clone_list;
                }
                case mal::Data::Type::Vector:  // This also doesn't leak
                {
                        auto* clone_vec = new mal::CloneVector(Clone);
                        for (auto& ptr : *mal_data->vector())
                        {
                                clone_vec->push(clone(ptr.get()));
                        }
                        return clone_vec;
                }
                default:
                        return mal_data;
        }
}

}  // namespace eval
