#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace mal
{

class Data;
class Symbol;
class Function;
using DataPtr = std::unique_ptr<mal::Data>;

class Environment
{
public:
        Environment();

        Environment(Environment const& other) = default;
        Environment& operator=(Environment const& other) = default;

        Environment(Environment&& other) = default;
        Environment& operator=(Environment&& other) = default;

        ~Environment()
        {
                for (auto& op : m_env)
                        delete op.first;
        }

        auto lookup(mal::Data* key) const -> std::optional<mal::Function*>;

private:
        struct FnHasher
        {
                std::size_t operator()(const mal::Data* key) const noexcept;
        };

        struct FnPred
        {
                bool operator()(const mal::Symbol* lhs, const mal::Symbol* rhs) const;
        };

        using EnvMap = std::unordered_map<mal::Symbol*, mal::DataPtr,
                                          mal::Environment::FnHasher, mal::Environment::FnPred>;

        EnvMap m_env{};
};

}  // namespace mal

#endif  // ENVIRONMENT_HPP
