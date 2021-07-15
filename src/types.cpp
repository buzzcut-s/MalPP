#include "../include/types.hpp"

#include <iostream>
#include <string>

namespace mal
{

std::string List::format() const
{
        std::string out = "(";
        for (auto* data : m_list)
        {
                out.append(data->format());
                out.append(" ");
        }

        if (!m_list.empty())
                out.back() = ')';
        else
                out.append(")");

        return out;
}

std::string Vector::format() const
{
        std::string out = "[";
        for (auto* data : m_vector)
        {
                out.append(data->format());
                out.append(" ");
        }

        if (!m_vector.empty())
                out.back() = ']';
        else
                out.append("]");

        return out;
}

}  // namespace mal
