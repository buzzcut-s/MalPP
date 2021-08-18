#include "../include/types.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace mal
{

// TODO(piush) Evaluate destructors
// Fixes Functiion::apply() memory leak

EvalList::~EvalList()
{
        for (auto* ptr : m_eval_list)
        {
                if (ptr && ptr->alloc_type() == mal::Data::AllocType::Nude)
                        delete ptr;
        }
        m_eval_list.clear();
}

CloneList::~CloneList()
{
        for (auto* ptr : m_clone_list)
        {
                if (ptr && ptr->alloc_type() == mal::Data::AllocType::Clone)
                        delete ptr;
        }
        m_clone_list.clear();
}

EvalVector::~EvalVector()
{
        for (auto* ptr : m_eval_vec)
        {
                if (ptr && ptr->alloc_type() == mal::Data::AllocType::Nude)
                        delete ptr;
        }
        m_eval_vec.clear();
}

CloneVector::~CloneVector()
{
        for (auto* ptr : m_clone_vec)
        {
                if (ptr && ptr->alloc_type() == mal::Data::AllocType::Clone)
                        delete ptr;
        }
        m_clone_vec.clear();
}

EvalHashMap::~EvalHashMap()
{
        for (auto& [key, val] : m_eval_map)
        {
                if (val && val->alloc_type() == mal::Data::AllocType::Nude)
                        delete val;
        }
        m_eval_map.clear();
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

std::string CloneList::format() const
{
        std::string out = "(";
        for (const auto& data : m_clone_list)
        {
                out.append(data->format());
                out.append(" ");
        }

        if (!m_clone_list.empty())
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

std::string CloneVector::format() const
{
        std::string out = "[";
        for (const auto& data : m_clone_vec)
        {
                out.append(data->format());
                out.append(" ");
        }

        if (!m_clone_vec.empty())
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

Vector* Data::vector()
{
        assert(this->type() == Type::Vector);
        return static_cast<Vector*>(this);
}

HashMap* Data::hashmap()
{
        assert(this->type() == Type::HashMap);
        return static_cast<HashMap*>(this);
}

Integer* Data::integer()
{
        assert(this->type() == Type::Integer);
        return static_cast<Integer*>(this);
}

EvalList* Data::eval_list()
{
        assert(this->type() == Type::EvalList);
        return static_cast<EvalList*>(this);
}

EvalVector* Data::eval_vector()
{
        assert(this->type() == Type::EvalVector);
        return static_cast<EvalVector*>(this);
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

}  // namespace mal
