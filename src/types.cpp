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

        if (!m_list.empty())
                out.back() = ')';
        else
                out.append(")");

        return out;
}

// TODO(piyush): implement
std::string Symbol::format()
{
        return symbol();
}

}  // namespace mal
