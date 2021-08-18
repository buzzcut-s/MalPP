#include "../include/Data.hpp"

#include <cassert>

#include "../include/types.hpp"

namespace mal
{

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
