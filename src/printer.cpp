#include "../include/printer.hpp"

#include <string>

#include "../include/types.hpp"

std::string pr_str(mal::Data* result)
{
        if (result)
                return result->format();

        // TODO(piyush): Handle this
        return "";
}
