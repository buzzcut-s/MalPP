#include "../include/Environment.hpp"

#include <cassert>
#include <iostream>
#include <optional>

#include "../include/types.hpp"

namespace mal
{
using Fn = std::function<mal::Data*(const std::size_t argc, mal::Data* const* args)>;

Environment::Environment(const Environment* outer, mal::List* binds, mal::List* exprs) :
    m_outer(outer)
{
        assert(binds->size() == exprs->size());
        for (size_t i = 0; i < binds->size(); ++i)
        {
                auto* sym_key  = binds->at(i)->symbol();
                auto* mal_data = exprs->at(i);
                set(sym_key, mal_data);
        }
}

void Environment::init(std::unordered_map<std::string, Fn> core_ns)
{
        for (const auto& [sym_key, mal_fn] : core_ns)
                set(new mal::Symbol(sym_key), new mal::Function(mal_fn));
}

void Environment::set(const mal::Symbol* sym_key, mal::Data* mal_data)
{
        m_env[sym_key] = mal_data;
}

const Environment* Environment::find_env(const mal::Symbol* sym_key) const
{
        if (auto res = m_env.find(sym_key); res != m_env.cend())
                return this;
        if (m_outer)
                return m_outer->find_env(sym_key);
        return nullptr;
}

auto Environment::lookup(const mal::Symbol* sym_key) const -> mal::Data*
{
        if (const auto* env = find_env(sym_key); env)
                return env->m_env.at(sym_key);

        // TODO(piyush) Handle an exception here
        std::cerr << "'" << sym_key->value() << "' not found";
        return nullptr;
}

// TODO(piyush) Implement this, for real (equality)
bool Environment::FnPred::operator()(const mal::Symbol* lhs, const mal::Symbol* rhs) const
{
        return lhs->value() == rhs->value();
}

std::size_t Environment::FnHasher::operator()(const mal::Symbol* sym_key) const noexcept
{
        return std::hash<std::string>{}(sym_key->value());
}
}  // namespace mal
