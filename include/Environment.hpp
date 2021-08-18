#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace mal
{
class Data;
class List;
class Symbol;

class Environment
{
public:
        Environment() = delete;

        explicit Environment(const Environment* outer) :
            m_outer(outer)
        {}

        Environment(const Environment* outer, mal::List* binds, mal::List* exprs);

        Environment(Environment const& other) = default;
        Environment& operator=(Environment const& other) = default;

        Environment(Environment&& other) = default;
        Environment& operator=(Environment&& other) = default;

        auto begin() { return m_env.begin(); }
        auto end() { return m_env.end(); }

        ~Environment() = default;

        using Fn = std::function<mal::Data*(const std::size_t argc, mal::Data* const* args)>;
        void init(std::unordered_map<std::string, Fn> core_ns);

        void set(const mal::Symbol* sym_key, mal::Data* mal_data);

        auto find_env(const mal::Symbol* sym_key) const -> const Environment*;
        auto lookup(const mal::Symbol* sym_key) const -> mal::Data*;

private:
        struct FnHasher
        {
                std::size_t operator()(const mal::Symbol* sym_key) const noexcept;
        };

        struct FnPred
        {
                bool operator()(const mal::Symbol* lhs, const mal::Symbol* rhs) const;
        };

        using EnvMap = std::unordered_map<const mal::Symbol*, mal::Data*,
                                          mal::Environment::FnHasher, mal::Environment::FnPred>;

        EnvMap             m_env{};
        const Environment* m_outer;
};

}  // namespace mal

#endif  // ENVIRONMENT_HPP
