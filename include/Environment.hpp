#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <optional>
#include <string>
#include <unordered_map>

#include "types.hpp"

namespace mal
{

class Environment
{
public:
        Environment();

        auto lookup(mal::Data* key) const -> std::optional<mal::Function*>;

private:
        struct FnHasher
        {
                std::size_t operator()(const mal::Data* key) const noexcept
                {
                        return std::hash<std::string>{}(key->format());
                }
        };

        struct FnPred
        {
                // TODO(piyush) Implement this, for real (equality)
                bool operator()(const mal::Symbol* lhs, const mal::Symbol* rhs) const
                {
                        // TODO(piyush) Changed this to check with string values. Ok?
                        return lhs->format() == rhs->format();
                }
        };

        using EnvMap = std::unordered_map<mal::Symbol*, mal::DataPtr,
                                          mal::Environment::FnHasher, mal::Environment::FnPred>;

        EnvMap m_env{};
};

}  // namespace mal

#endif  // ENVIRONMENT_HPP
