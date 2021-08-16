#include "../include/types.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace mal
{

Integer* Data::integer()
{
        assert(this->type() == Type::Integer);
        return static_cast<Integer*>(this);
}

Symbol* Data::symbol()
{
        assert(this->type() == Type::Symbol);
        return static_cast<Symbol*>(this);
}

List* Data::list()
{
        assert(this->type() == Type::List);
        return static_cast<List*>(this);
}

EvalList* Data::eval_list()
{
        assert(this->type() == Type::EvalList);
        return static_cast<EvalList*>(this);
}

Vector* Data::vector()
{
        assert(this->type() == Type::Vector);
        return static_cast<Vector*>(this);
}

EvalVector* Data::eval_vector()
{
        assert(this->type() == Type::EvalVector);
        return static_cast<EvalVector*>(this);
}

HashMap* Data::hashmap()
{
        assert(this->type() == Type::HashMap);
        return static_cast<HashMap*>(this);
}

EvalHashMap* Data::eval_hashmap()
{
        assert(this->type() == Type::EvalHashMap);
        return static_cast<EvalHashMap*>(this);
}

Function* Data::function()
{
        assert(this->type() == Type::Function);
        return static_cast<Function*>(this);
}

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

std::string EvalList::format() const
{
        std::string out = "(";
        for (const auto& data : m_eval_list)
        {
                out.append(data->format());
                out.append(" ");
        }

        if (!m_eval_list.empty())
                out.back() = ')';
        else
                out.append(")");

        return out;
}

std::string Vector::format() const
{
        std::string out = "[";
        for (const auto& data : m_vec)
        {
                out.append(data->format());
                out.append(" ");
        }

        if (!m_vec.empty())
                out.back() = ']';
        else
                out.append("]");

        return out;
}

std::string EvalVector::format() const
{
        std::string out = "[";
        for (const auto& data : m_eval_vec)
        {
                out.append(data->format());
                out.append(" ");
        }

        if (!m_eval_vec.empty())
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

std::string EvalHashMap::format() const
{
        std::string out = "{";
        for (const auto& [key, value] : m_eval_map)
        {
                out.append(key->format());
                out.append(" ");
                out.append(value->format());
                out.append(" ");
        }

        if (!m_eval_map.empty())
                out.back() = '}';
        else
                out.append("}");

        return out;
}

}  // namespace mal
