#include "../include/mal.hpp"

#include <functional>
#include <string>
#include <utility>

#include "../include/Reader.hpp"
#include "../include/printer.hpp"
#include "../include/types.hpp"

mal::Data* READ(std::string input)
{
        return read_str(std::move(input));
}

// TODO(piyush) Implement this.
mal::Data* EVAL(mal::Data* ast, const repl_env& env)
{
        auto evaluated = eval_ast(ast, env);
        return evaluated;
}

// TODO(piyush) Implement this.
mal::Data* eval_ast(mal::Data* ast, const repl_env& env)
{
        // TODO(piyush) This is the trickiest part yet
        // TODO(piyush) How to pass arguments to lambda from env?????
        // TODO(piyush) I have no idea.
        // TODO(piyush) AND RECURSION ON TOP OF THAT
        // TODO(piyush) /:
        return ast;
}

std::string PRINT(mal::Data* input)
{
        return pr_str(input);
}

std::string rep(std::string input)
{
        // TODO(piyush) Not sure where all these lambdas should go
        // TODO(piyush) Should be mal::Data* ?
        static const int_fn ADD_LAMBDA =
            [](const mal::Integer* lhs, const mal::Integer* rhs) -> mal::Integer* {
                return new mal::Integer{*lhs + *rhs};
        };

        static const int_fn SUBTRACT_LAMBDA =
            [](const mal::Integer* lhs, const mal::Integer* rhs) -> mal::Integer* {
                return new mal::Integer{*lhs - *rhs};
        };

        static const int_fn MULTIPLY_LAMBDA =
            [](const mal::Integer* lhs, const mal::Integer* rhs) -> mal::Integer* {
                return new mal::Integer{*lhs * *rhs};
        };

        static const int_fn DIVIDE_LAMBDA =
            [](const mal::Integer* lhs, const mal::Integer* rhs) -> mal::Integer* {
                return new mal::Integer{*lhs / *rhs};
        };

        // TODO(piyush) This looks pretty neat
        // TODO(piyush) .find() symbol in env and call ->second()
        // TODO(piyush) ->second() IS THIS WHERE I PASS THE ARGUMENTS? I THINK SO!
        static const repl_env ENV{
            {new mal::Symbol{"+"}, ADD_LAMBDA},
            {new mal::Symbol{"-"}, SUBTRACT_LAMBDA},
            {new mal::Symbol{"*"}, MULTIPLY_LAMBDA},
            {new mal::Symbol{"/"}, DIVIDE_LAMBDA},
        };

        auto* ast    = READ(std::move(input));
        auto* result = EVAL(ast, ENV);

        return PRINT(result);
}
