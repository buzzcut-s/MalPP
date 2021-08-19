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
class Vector;
class Symbol;

class Environment
{
public:
        Environment() = delete;

        explicit Environment(const Environment* outer) :
            m_outer(outer)
        {}

        Environment(const Environment* outer, mal::List* binds, mal::List* exprs);
        Environment(const Environment* outer, mal::Vector* binds, mal::List* exprs);

        Environment(Environment const& other) = default;
        Environment& operator=(Environment const& other) = default;

        Environment(Environment&& other) = default;
        Environment& operator=(Environment&& other) = default;

        ~Environment() = default;

        auto begin() { return m_env.begin(); }
        auto end() { return m_env.end(); }

        void set(const mal::Symbol* sym_key, mal::Data* mal_data);

        auto find_env(const mal::Symbol* sym_key) const -> const Environment*;
        auto lookup(const mal::Symbol* sym_key) const -> mal::Data*;

        using Fn = std::function<mal::Data*(const std::size_t argc, mal::Data* const* args)>;
        void init(std::unordered_map<std::string, Fn> core_ns);
        void uninit();

private:
        void init_binds(mal::List* binds, mal::List* exprs);
        void init_binds(mal::Vector* binds, mal::List* exprs);

        void set_var_binds(mal::Symbol* var_key, mal::List* var_exprs, size_t var_idx);

        struct FnHasher
        {
                size_t operator()(const mal::Symbol* sym_key) const noexcept;
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
