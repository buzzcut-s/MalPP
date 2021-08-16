#include "../include/Environment.hpp"

#include <cassert>
#include <optional>

#include "../include/types.hpp"

mal::Environment::~Environment()
{
        for (auto& [symbol, fn] : m_env)
                delete symbol;
}

// TODO(piyush) Implement this, for real (equality)
bool mal::Environment::FnPred::operator()(const mal::Symbol* lhs, const mal::Symbol* rhs) const
{
        // TODO(piyush) Changed this to check with string values. Ok?
        return lhs->format() == rhs->format();
}

std::size_t mal::Environment::FnHasher::operator()(const mal::Symbol* key) const noexcept
{
        return std::hash<std::string>{}(key->format());
}

mal::Environment::Environment()
{
        using Fn = std::function<mal::Data*(std::size_t argc, mal::Data* const* args)>;

        Fn add_impl = [](size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(lhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() + *rhs->integer();
                // TODO(piyush) Memory leak :D
                return new mal::Integer(res);
        };

        Fn subtract_impl = [](size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(lhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() - *rhs->integer();
                // TODO(piyush) Memory leak :D
                return new mal::Integer(res);
        };

        Fn multiply_impl = [](size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(lhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() * *rhs->integer();
                // TODO(piyush) Memory leak :D
                return new mal::Integer(res);
        };

        Fn divide_impl = [](size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(lhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() / *rhs->integer();
                // TODO(piyush) Memory leak :D
                return new mal::Integer(res);
        };

        m_env.emplace(new mal::Symbol("+"),
                      std::make_unique<mal::Function>(add_impl));
        m_env.emplace(new mal::Symbol("-"),
                      std::make_unique<mal::Function>(subtract_impl));
        m_env.emplace(new mal::Symbol("*"),
                      std::make_unique<mal::Function>(multiply_impl));
        m_env.emplace(new mal::Symbol("/"),
                      std::make_unique<mal::Function>(divide_impl));
};

auto mal::Environment::lookup(mal::Data* key) const -> std::optional<mal::Function*>
{
        if (auto res = m_env.find(key->symbol()); res != m_env.cend())
                return res->second->function();
        return std::nullopt;
}
