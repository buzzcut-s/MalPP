#include "../../include/types/Keyword.hpp"

namespace mal
{

bool Keyword::operator==(Data* rhs)
{
        // TODO(piyush) Check for nullptr
        return rhs->type() == mal::Data::Type::Keyword
               && this->value() == rhs->keyword()->value();
}

}  // namespace mal
