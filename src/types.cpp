#include "../include/types.hpp"

#include <iostream>
#include <string>

namespace mal
{

std::string List::format() const
{
        std::string out = "(";
        for (const auto& data : m_list)
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
        for (const auto& data : m_vector)
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

std::string HashMap::format() const
{
        std::string out = "{";
        for (const auto& [key, value] : m_hashmap)
        {
                out.append(key->format());
                out.append(" ");
                out.append(value->format());
                out.append(" ");
        }

        if (!m_hashmap.empty())
                out.back() = '}';
        else
                out.append("}");

        return out;
}

}  // namespace mal
