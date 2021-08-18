#include "../../include/types/Vector.hpp"

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

}  // namespace mal
