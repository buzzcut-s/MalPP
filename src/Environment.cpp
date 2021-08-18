#include "../include/Environment.hpp"

#include <cassert>
#include <iostream>
#include <optional>

#include "../include/types.hpp"

namespace mal
{

Environment::Environment(Environment* outer, mal::List* binds, mal::FnList* exprs) :
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

Environment::~Environment()
{
        for (auto& [symbol_key, fn] : m_env)
        {
                if (fn && fn->alloc_type() == mal::Data::AllocType::Nude)
                        delete fn;
                if (symbol_key && symbol_key->alloc_type() == mal::Data::AllocType::Nude)
                        delete symbol_key;
        }
        m_env.clear();
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

        using mal::Data::AllocType::Init;
        set(new mal::Symbol("+", Init), new mal::Function(add_impl, Init));
        set(new mal::Symbol("-", Init), new mal::Function(subtract_impl, Init));
        set(new mal::Symbol("*", Init), new mal::Function(multiply_impl, Init));
        set(new mal::Symbol("/", Init), new mal::Function(divide_impl, Init));
}

void Environment::uninit()
{
        for (auto& [symbol_key, fn] : m_env)
        {
                if (fn && fn->alloc_type() == mal::Data::AllocType::Init)
                        delete fn;
                if (symbol_key && symbol_key->alloc_type() == mal::Data::AllocType::Init)
                        delete symbol_key;
        }
        m_env.clear();
}

void Environment::clear_inner()
{
        delete m_inner;
        m_inner = nullptr;
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
