#include "../include/mal.hpp"

#include <iostream>
#include <string>
#include <utility>

#include "../include/Reader.hpp"
#include "../include/printer.hpp"
#include "../include/types.hpp"

mal::Data* READ(std::string input)
{
        return read_str(std::move(input));
}

mal::Data* EVAL(mal::Data* value)
{
        return value;
}

std::string PRINT(mal::Data* input)
{
        return pr_str(input);
}

std::string rep(std::string input)
{
        auto* ast    = READ(std::move(input));
        auto* result = EVAL(ast);

        return PRINT(result);
}
