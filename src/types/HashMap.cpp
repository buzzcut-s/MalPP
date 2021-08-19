#include "../../include/types/HashMap.hpp"

namespace mal
{

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
        // TODO(piyush) Check for nullptr
        switch (rhs->type())
        {
                case mal::Data::Type::HashMap:
                {
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

                default:
                        return false;
        }
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

}  // namespace mal
