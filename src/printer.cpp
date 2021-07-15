#include "../include/printer.hpp"

#include <string>

#include "../include/types.hpp"

std::string pr_str(mal::Data* value)
{
        if (value)
                return value->format();

        // TODO(piyush): Handle this
        return "";
}
