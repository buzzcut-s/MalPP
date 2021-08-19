#include "../../include/types/String.hpp"

namespace mal
{

bool String::operator==(Data* rhs)
{
        // TODO(piyush) Check for nullptr
        return rhs->type() == mal::Data::Type::String
               && this->value() == rhs->string()->value();
}

}  // namespace mal
