#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

// TODO(piyush) Can we forward declare relevant types here?
// Doesn't compile since I shifted env to main without this
#include "types.hpp"

namespace mal
{

class Environment
{
public:
        Environment() = delete;

        explicit Environment(Environment* outer) :
            m_outer(outer)
        {}

        Environment(Environment const& other) = default;
        Environment& operator=(Environment const& other) = default;

        Environment(Environment&& other) = default;
        Environment& operator=(Environment&& other) = default;

        ~Environment();

        void init();

        void set(const mal::Symbol* symbol_key, mal::DataPtr mal_fn);

        auto lookup(mal::Data* symbol_key) const -> std::optional<mal::Function*>;

private:
        struct FnHasher
        {
                std::size_t operator()(const mal::Symbol* symbol_key) const noexcept;
        };

        struct FnPred
        {
                bool operator()(const mal::Symbol* lhs, const mal::Symbol* rhs) const;
        };

        using EnvMap = std::unordered_map<const mal::Symbol*, const mal::DataPtr,
                                          mal::Environment::FnHasher, mal::Environment::FnPred>;

        Environment* m_outer;
        EnvMap       m_env{};
};

}  // namespace mal

#endif  // ENVIRONMENT_HPP
