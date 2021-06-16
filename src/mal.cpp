#include "../include/mal.hpp"

#include <iostream>
#include <string>

#include "../include/Reader.hpp"
#include "../include/printer.hpp"
#include "../include/types.hpp"

mal::Data* READ(std::string_view input)
{
        return read_str(input);
}

mal::Data* EVAL(mal::Data* value)
{
        return value;
}

std::string PRINT(mal::Data* input)
{
        return pr_str(input);
}

std::string rep(std::string_view input)
{
        auto* ast    = READ(input);
        auto* result = EVAL(ast);

        return PRINT(result);
}
