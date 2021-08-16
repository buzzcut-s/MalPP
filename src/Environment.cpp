#include "../include/Environment.hpp"

#include <cassert>
#include <optional>

#include "../include/types.hpp"

mal::Environment::Environment()
{
        static const std::function<mal::Data*(const size_t argc, mal::Data* const* args)>
            add_impl = [](size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(lhs->type() == mal::Data::Type::Integer);

                auto res = lhs->integer()->value() + rhs->integer()->value();
                return new mal::Integer(res);
        };

        static const std::function<mal::Data*(const size_t argc, mal::Data* const* args)>
            subtract_impl = [](size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(lhs->type() == mal::Data::Type::Integer);

                auto res = lhs->integer()->value() - rhs->integer()->value();
                return new mal::Integer(res);
        };

        static const std::function<mal::Data*(const size_t argc, mal::Data* const* args)>
            multiply_impl = [](size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(lhs->type() == mal::Data::Type::Integer);

                auto res = lhs->integer()->value() * rhs->integer()->value();
                return new mal::Integer(res);
        };

        static const std::function<mal::Data*(const size_t argc, mal::Data* const* args)>
            divide_impl = [](size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(lhs->type() == mal::Data::Type::Integer);

                auto res = lhs->integer()->value() / rhs->integer()->value();
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
