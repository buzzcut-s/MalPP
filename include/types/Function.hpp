#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <functional>
#include <string>
#include <utility>

#include "../Data.hpp"

namespace mal
{

class Function : public Data
{
public:
        using Fn = std::function<mal::Data*(const std::size_t argc, mal::Data* const* args)>;

        Function() = delete;

        explicit Function(Fn fn) :
            m_fn{std::move(fn)}
        {}

        Function(Function const& other) = default;
        Function& operator=(Function const& other) = delete;

        Function(Function&& other) = default;
        Function& operator=(Function&& other) = delete;

        ~Function() override = default;

        [[nodiscard]] std::string format() const override { return "#<function>"; }

        [[nodiscard]] Type type() const override { return Type::Function; }

        [[nodiscard]] auto value() const { return m_fn; }

        auto apply(const size_t argc, mal::Data* const* args) const { return m_fn(argc, args); }

private:
        Fn m_fn{};
};

}  // namespace mal

#endif  // FUNCTION_HPP
