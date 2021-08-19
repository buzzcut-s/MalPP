#include "../../include/types/Integer.hpp"

namespace mal
{

bool Integer::operator==(Data* rhs)
{
        // TODO(piyush) Check for nullptr
        return rhs->type() == mal::Data::Type::Integer
               && this->value() == rhs->integer()->value();
}

}  // namespace mal
