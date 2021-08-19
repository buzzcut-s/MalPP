#include "../../include/types/Vector.hpp"

#include <iostream>

#include "../../include/types/List.hpp"

namespace mal
{

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

bool Vector::operator==(Data* rhs)
{
        // TODO(piyush) Check for nullptr
        switch (rhs->type())
        {
                case mal::Data::Type::List:
                {
                        if (this->size() != rhs->list()->size())
                                return false;
                        for (size_t i = 0; i < this->size(); ++i)
                        {
                                if (*this->at(i) != rhs->list()->at(i))
                                        return false;
                        }
                        return true;
                }

                case mal::Data::Type::Vector:
                {
                        if (this->size() != rhs->vector()->size())
                                return false;
                        for (size_t i = 0; i < this->size(); ++i)
                        {
                                if (*this->at(i) != rhs->vector()->at(i))
                                        return false;
                        }
                        return true;
                }

                default:
                {
                        std::cerr << "vector operator== default return\n";
                        return false;
                }
        }
}

}  // namespace mal
