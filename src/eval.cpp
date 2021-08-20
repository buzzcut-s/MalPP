#include "../include/eval.hpp"

#include <cassert>
#include <iostream>

#include "../include/Environment.hpp"
#include "../include/mal.hpp"
#include "../include/types.hpp"

namespace eval
{

// implementation specific functions
static mal::Data* eval_let_list(mal::List* uneval_list, mal::Environment& new_env);

static mal::Data* eval_let_vec(mal::List* uneval_list, mal::Environment& new_env);

static mal::Data* eval_fn_list(mal::List* uneval_list, const mal::Environment& repl_env);

static mal::Data* eval_fn_vec(mal::List* uneval_list, const mal::Environment& repl_env);

// switches on the type of ast as follows :
// symbol    :  lookup the symbol in the environment structure and return the value (or error)
// list (or vector) :return a new List (or Vector)
//                  that is the result of calling EVAL on each of the member of ast list (or vector)
// hashmap   : return a new HashMap which consists of key-value pairs where
//             key   - a a key from the ast HashMap
//             value - the result of calling EVAL on the corresponding value
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

// default case in EVAL = ast is a List, and first item is not a Symbol :
// - call eval_ast to get a new evaluated list (args)
// - take the first item of the evaluated list (mal_fn)
// - call it as function (apply) using the rest of the evaluated list as its arguments
mal::Data* eval_apply(mal::Data* ast, mal::Environment& repl_env)
{
        auto* args = eval::eval_ast(ast, repl_env)->list();
        if (args->front() && args->front()->type() == mal::Data::Type::Function)
        {
                auto* mal_fn = args->front()->function();
                return mal_fn->apply(args->size() - 1, args->data() + 1);
        }
        std::cerr << "eval apply - from EVAL default\n";
        return nullptr;
}

// calls the set method of the current environment (second parameter of EVAL called env) with :
// symbol key : the unevaluated first parameter (second list element)
// value      : the evaluated second parameter as the value
// also returns the value which is printed
mal::Data* eval_def(mal::List* uneval_list, mal::Environment& repl_env)
{
        auto* sym_key = uneval_list->at(1);
        if (auto* mal_data = EVAL(uneval_list->at(2), repl_env); mal_data)
        {
                repl_env.set(sym_key->symbol(), mal_data);
                return mal_data;
        }
        std::cerr << "eval def\n";
        return nullptr;
}

// creates a new environment using the current environment as the outer value
// use the first parameter as a List (or Vector) of new bindings in the new "let*" environment
mal::Data* eval_let(mal::List* uneval_list, mal::Environment& repl_env)
{
        auto* new_env = new mal::Environment(&repl_env);

        if (uneval_list->at(1)->type() == mal::Data::Type::List)
                return eval_let_list(uneval_list, *new_env);

        if (uneval_list->at(1)->type() == mal::Data::Type::Vector)
                return eval_let_vec(uneval_list, *new_env);

        std::cerr << "eval let\n";
        return nullptr;
}

// evaluate all the elements of the list using eval_ast and return the final evaluated element
mal::Data* eval_do(mal::List* uneval_list, mal::Environment& repl_env)
{
        mal::Data* result{};
        assert(uneval_list->size() > 1);
        for (size_t i = 1; i < uneval_list->size(); ++i)
                result = EVAL(uneval_list->at(i), repl_env);
        return result;
}

// evaluate the first parameter (second element), which is the condition
// if condition is truthy (not Nil or False)
// - then evaluate the second parameter (third element of the list) and return the result
// else
// - evaluate the third parameter (fourth element) and return the resul
// ff condition is false and there is no third parameter, then return nil
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

// return a new Function closure
// calls the respective impl functions for List or Vector
mal::Data* eval_fn(mal::List* uneval_list, const mal::Environment& repl_env)
{
        if (uneval_list->at(1)->type() == mal::Data::Type::List)
                return eval_fn_list(uneval_list, repl_env);

        if (uneval_list->at(1)->type() == mal::Data::Type::Vector)
                return eval_fn_vec(uneval_list, repl_env);

        std::cerr << "eval fn\n";
        return nullptr;
}

// eval_let where first parameter was a list
// takes the second element of the binding list,
// and calls EVAL using the new "let*" environment as the evaluation env
// calls set on the new "let*" env using
//      key -   the first binding list element as the symbol key
//      value - the evaluated second element as the value
// this is repeated for each odd/even pair in the binding list
// finally, the second parameter (third element) of the original let* form is evaluated,
// using the new "let*" env and the result is returned
// the new env is discarded upcon completion
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

// the same as eval_let_list but for vectors
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

// returns a new Function closure, body of which does the following :
// - creates a new environment using repl_env (closed over from outer scope) as the outer paramter
// - the first parameter (second list element of ast from the outer scope) as the binds parameter
// - the parameters to the closure as the exprs parameter
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

// // the same as eval_fn_list but for vectors
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
