#include "../include/types.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace mal
{

Integer* Data::integer()
{
        assert(this->type() == Type::Integer);
        return dynamic_cast<Integer*>(this);
}

Symbol* Data::symbol()
{
        assert(this->type() == Type::Symbol);
        return dynamic_cast<Symbol*>(this);
}

List* Data::list()
{
        assert(this->type() == Type::List);
        return dynamic_cast<List*>(this);
}

EvalList* Data::eval_list()
{
        assert(this->type() == Type::EvalList);
        return dynamic_cast<EvalList*>(this);
}

Vector* Data::vector()
{
        assert(this->type() == Type::Vector);
        return dynamic_cast<Vector*>(this);
}

EvalVector* Data::eval_vector()
{
        assert(this->type() == Type::EvalVector);
        return dynamic_cast<EvalVector*>(this);
}

Function* Data::function()
{
        assert(this->type() == Type::Function);
        return dynamic_cast<Function*>(this);
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

}  // namespace mal
