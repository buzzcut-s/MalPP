#include "../../include/types/Keyword.hpp"

namespace mal
{

bool Keyword::operator==(Data* rhs)
{
        return rhs->type() == mal::Data::Type::Keyword
               && this->value() == rhs->keyword()->value();
}

}  // namespace mal
