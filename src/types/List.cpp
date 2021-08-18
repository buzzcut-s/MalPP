#include "../../include/types/List.hpp"

#include <iostream>

#include "../../include/types/Vector.hpp"

namespace mal
{

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
                        std::cerr << "list operator== default return\n";
                        return false;
                }
        }
}

}  // namespace mal
