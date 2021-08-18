#include "../include/types.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace mal
{

bool Integer::operator==(Data* rhs)
{
        return rhs->type() == mal::Data::Type::Integer
               && this->value() == rhs->integer()->value();
}

bool String::operator==(Data* rhs)
{
        return rhs->type() == mal::Data::Type::String
               && this->value() == rhs->string()->value();
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

bool List::operator==(Data* rhs)
{
        if (rhs->type() != mal::Data::Type::List)
                return false;
        if (this->size() != rhs->list()->size())
                return false;
        for (size_t i = 0; i < this->size(); ++i)
        {
                if (*this->at(i) != rhs->list()->at(i))
                        return false;
        }
        return true;
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

bool HashMap::operator==(Data* rhs)
{
        if (rhs->type() != mal::Data::Type::HashMap)
                return false;
        if (this->size() != rhs->hashmap()->size())
                return false;
        for (auto [this_key, this_val] : *this)
        {
                auto* rhs_val = rhs->hashmap()->find(this_key);
                if (!rhs_val || *rhs_val != this_val)
                        return false;
        }
        return true;
}

auto HashMap::find(mal::Data* key) const -> mal::Data*
{
        if (auto res = m_hashmap.find(key); res != m_hashmap.cend())
                return res->second;
        return nullptr;
}

std::size_t HashMap::DataHasher::operator()(const mal::Data* key) const noexcept
{
        return std::hash<std::string>{}(key->format());
}

// TODO(piyush) Implement this, for real (equality)
bool HashMap::DataPred::operator()(const mal::Data* lhs, const mal::Data* rhs) const
{
        // TODO(piyush) Changed this to check with string values. Ok?
        return lhs->format() == rhs->format();
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

String* Data::string()
{
        assert(this->type() == Type::String);
        return static_cast<String*>(this);
}

Function* Data::function()
{
        assert(this->type() == Type::Function);
        return static_cast<Function*>(this);
}

}  // namespace mal
