#include "../include/mal.hpp"

#include <iostream>
#include <string>

const std::string& READ(const std::string& input)
{
        return input;
}

const std::string& EVAL(const std::string& ast)
{
        return ast;
}

const std::string& PRINT(const std::string& ast)
{
        return ast;
}

const std::string& rep(const std::string& input)
{
        return PRINT(EVAL(READ(input)));
}
