#ifndef NIL_HPP
#define NIL_HPP

#include <string>

#include "../Data.hpp"

namespace mal
{

class Nil : public Data
{
public:
        Nil() = default;

        Nil(Nil const& other) = default;
        Nil& operator=(Nil const& other) = default;

        Nil(Nil&& other) = default;
        Nil& operator=(Nil&& other) = default;

        ~Nil() override = default;

        [[nodiscard]] std::string format() const override { return "nil"; }

        [[nodiscard]] Type type() const override { return Type::Nil; }

        [[nodiscard]] bool is_truthy() const override { return false; }

        // TODO(piyush) Check for nullptr
        bool operator==(Data* rhs) override { return rhs->type() == Type::Nil; }
};

}  // namespace mal

#endif  // NIL_HPP
