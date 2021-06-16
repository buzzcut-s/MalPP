#include "../include/printer.hpp"

#include "../include/types.hpp"

std::string pr_str(mal::Data* value)
{
        if (value)
                return value->format();
}
