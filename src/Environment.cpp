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
        init_binds(binds, exprs);
}

Environment::Environment(const Environment* outer, mal::Vector* binds, mal::List* exprs) :
    m_outer(outer)
{
        init_binds(binds, exprs);
}

void Environment::init_binds(mal::List* binds, mal::List* exprs)
{
        for (size_t i = 0; i < binds->size(); ++i)
        {
                auto* sym_key = binds->at(i)->symbol();
                if (sym_key->is_variadic())
                {
                        if (i + 1 >= binds->size())
                        {
                                std::cerr << "missing symbol after &\n";
                                return;
                        }
                        auto* var_key = binds->at(i + 1)->symbol();
                        set_var_binds(var_key, exprs, i);
                        return;
                }
                if (i >= exprs->size())
                {
                        std::cerr << "not enough arguments\n";
                        return;
                }
                auto* mal_data = exprs->at(i);
                set(sym_key, mal_data);
        }
}

void Environment::init_binds(mal::Vector* binds, mal::List* exprs)
{
        for (size_t i = 0; i < binds->size(); ++i)
        {
                auto* sym_key = binds->at(i)->symbol();
                if (sym_key->is_variadic())
                {
                        if (i + 1 >= binds->size())
                        {
                                std::cerr << "missing symbol after &\n";
                                return;
                        }
                        auto* var_key = binds->at(i + 1)->symbol();
                        set_var_binds(var_key, exprs, i);
                        return;
                }
                if (i >= exprs->size())
                {
                        std::cerr << "not enough arguments\n";
                        return;
                }
                auto* mal_data = exprs->at(i);
                set(sym_key, mal_data);
        }
}

void Environment::set_var_binds(mal::Symbol* var_key, mal::List* var_exprs, size_t var_idx)
{
        auto* var_exprs_list = new mal::List;
        for (size_t i = var_idx; i < var_exprs->size(); ++i)
                var_exprs_list->push(var_exprs->at(i));
        set(var_key, var_exprs_list);
}

void Environment::init(std::unordered_map<std::string, Fn> core_ns)
{
        for (const auto& [core_key, core_fn] : core_ns)
                set(new mal::Symbol(core_key), new mal::Function(core_fn));
}

void Environment::uninit()
{
        for (auto [core_key, core_fn] : m_env)
        {
                delete core_key;
                delete core_fn;
        }
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
        std::cerr << "'" << sym_key->value() << "' not found\n";
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
