#include "../../include/types/List.hpp"

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

}  // namespace mal
