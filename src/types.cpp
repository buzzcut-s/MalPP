#include "../include/types.hpp"

#include <iostream>
#include <string>

namespace mal
{

std::string List::format()
{
        std::string out = "(";
        for (auto* value : m_list)
        {
                out.append(value->format());
                out.append(" ");
        }
        out.back() = ')';
        return out;
}

std::string Symbol::format()
{
        return "OK";
}

}  // namespace mal
