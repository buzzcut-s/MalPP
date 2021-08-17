#include "../include/Environment.hpp"

#include <cassert>
#include <optional>

#include "../include/types.hpp"

namespace mal
{

Environment::~Environment()
{
        for (auto& [symbol, fn] : m_env)
                delete symbol;
}

// TODO(piyush) Implement this, for real (equality)
bool Environment::FnPred::operator()(const mal::Symbol* lhs, const mal::Symbol* rhs) const
{
        // TODO(piyush) Changed this to check with string values. Ok?
        return lhs->format() == rhs->format();
}

std::size_t Environment::FnHasher::operator()(const mal::Symbol* key) const noexcept
{
        return std::hash<std::string>{}(key->format());
}

Environment::Environment()
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
                // TODO(piyush) Memory leak :D
                return new mal::Integer(res, Nude);
        };

        const Fn subtract_impl = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() - *rhs->integer();
                // TODO(piyush) Memory leak :D
                return new mal::Integer(res, Nude);
        };

        const Fn multiply_impl = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() * *rhs->integer();
                // TODO(piyush) Memory leak :D
                return new mal::Integer(res, Nude);
        };

        const Fn divide_impl = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() / *rhs->integer();
                // TODO(piyush) Memory leak :D
                return new mal::Integer(res, Nude);
        };

        m_env.emplace(new mal::Symbol("+", Nude),
                      std::make_unique<mal::Function>(add_impl));
        m_env.emplace(new mal::Symbol("-", Nude),
                      std::make_unique<mal::Function>(subtract_impl));
        m_env.emplace(new mal::Symbol("*", Nude),
                      std::make_unique<mal::Function>(multiply_impl));
        m_env.emplace(new mal::Symbol("/", Nude),
                      std::make_unique<mal::Function>(divide_impl));
};

auto Environment::lookup(mal::Data* key) const -> std::optional<mal::Function*>
{
        if (auto res = m_env.find(key->symbol()); res != m_env.cend())
                return res->second->function();
        return std::nullopt;
}

}  // namespace mal
