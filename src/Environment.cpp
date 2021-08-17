#include "../include/Environment.hpp"

#include <cassert>
#include <iostream>
#include <optional>

#include "../include/types.hpp"

namespace mal
{

Environment::~Environment()
{
        for (auto& [symbol_key, fn] : m_env)
        {
                delete fn;
                delete symbol_key;
        }
        delete m_inner;
}

void Environment::init()
{
        using Fn = std::function<mal::Data*(const std::size_t argc, mal::Data* const* args)>;
        using mal::Data::AllocType::Nude;

        const Fn add_impl = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() + *rhs->integer();
                return new mal::Integer(res, Nude);
        };

        const Fn subtract_impl = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() - *rhs->integer();
                return new mal::Integer(res, Nude);
        };

        const Fn multiply_impl = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() * *rhs->integer();
                return new mal::Integer(res, Nude);
        };

        const Fn divide_impl = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() / *rhs->integer();
                return new mal::Integer(res, Nude);
        };

        set(new mal::Symbol("+", Nude), new mal::Function(add_impl));
        set(new mal::Symbol("-", Nude), new mal::Function(subtract_impl));
        set(new mal::Symbol("*", Nude), new mal::Function(multiply_impl));
        set(new mal::Symbol("/", Nude), new mal::Function(divide_impl));
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
        // TODO(piyush) Changed this to check with string values. Ok?
        return lhs->value() == rhs->value();
}

std::size_t Environment::FnHasher::operator()(const mal::Symbol* sym_key) const noexcept
{
        return std::hash<std::string>{}(sym_key->value());
}
}  // namespace mal
