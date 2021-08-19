#include "../include/core.hpp"

#include <cassert>
#include <iostream>

#include "../include/printer.hpp"
#include "../include/types.hpp"

namespace core
{

std::unordered_map<std::string, Fn> ns_init()
{
        Fn core_add = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() + *rhs->integer();
                return new mal::Integer(res);
        };

        Fn core_subtract = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() - *rhs->integer();
                return new mal::Integer(res);
        };

        Fn core_multiply = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() * *rhs->integer();
                return new mal::Integer(res);
        };

        Fn core_divide = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                auto res = *lhs->integer() / *rhs->integer();
                return new mal::Integer(res);
        };

        Fn core_prn = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                if (argc < 1)
                        std::cout << "\n";
                else
                        std::cout << pr_str(args[0]) << "\n";

                return new mal::Nil;
        };

        Fn core_list = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                auto* list = new mal::List;
                for (size_t i = 0; i < argc; ++i)
                        list->push(args[i]);
                return list;
        };

        Fn core_list_q = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc >= 1);

                if (args[0]->type() == mal::Data::Type::List)
                        return new mal::True;
                return new mal::False;
        };

        Fn core_empty_q = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc >= 1);

                if (args[0]->type() == mal::Data::Type::List && args[0]->list()->empty())
                        return new mal::True;
                if (args[0]->type() == mal::Data::Type::Vector && args[0]->vector()->empty())
                        return new mal::True;
                return new mal::False;
        };

        Fn core_count = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc >= 1);

                if (args[0]->type() == mal::Data::Type::List)
                        return new mal::Integer(args[0]->list()->size());
                if (args[0]->type() == mal::Data::Type::Vector)
                        return new mal::Integer(args[0]->vector()->size());

                return new mal::Integer(0);
        };

        Fn core_eq = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                if (*lhs == rhs)
                        return new mal::True;
                return new mal::False;
        };

        Fn core_lt = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                if (lhs->integer()->value() < rhs->integer()->value())
                        return new mal::True;
                return new mal::False;
        };

        Fn core_lte = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                if (lhs->integer()->value() <= rhs->integer()->value())
                        return new mal::True;
                return new mal::False;
        };

        Fn core_gt = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                if (lhs->integer()->value() > rhs->integer()->value())
                        return new mal::True;
                return new mal::False;
        };

        Fn core_gte = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc == 2);
                const auto& lhs = args[0];
                const auto& rhs = args[1];

                assert(lhs->type() == mal::Data::Type::Integer);
                assert(rhs->type() == mal::Data::Type::Integer);

                if (lhs->integer()->value() >= rhs->integer()->value())
                        return new mal::True;
                return new mal::False;
        };

        Fn core_not = [](const size_t argc, mal::Data* const* args) -> mal::Data* {
                assert(argc >= 1);
                const auto& condition = args[0];

                if (condition->is_truthy())
                        return new mal::False;
                return new mal::True;
        };

        std::unordered_map<std::string, Fn> core_ns{
            {"+", std::move(core_add)},
            {"-", std::move(core_subtract)},
            {"*", std::move(core_multiply)},
            {"/", std::move(core_divide)},
            {"prn", std::move(core_prn)},
            {"list", std::move(core_list)},
            {"list?", std::move(core_list_q)},
            {"empty?", std::move(core_empty_q)},
            {"count", std::move(core_count)},
            {"=", std::move(core_eq)},
            {"<", std::move(core_lt)},
            {"<=", std::move(core_lte)},
            {">", std::move(core_gt)},
            {">=", std::move(core_gte)},
            {"not", std::move(core_not)},
        };

        return core_ns;
}

}  // namespace core
