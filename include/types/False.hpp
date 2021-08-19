#ifndef FALSE_HPP
#define FALSE_HPP

#include <string>

#include "../Data.hpp"

namespace mal
{

class False : public Data
{
public:
        False() = default;

        False(False const& other) = default;
        False& operator=(False const& other) = default;

        False(False&& other) = default;
        False& operator=(False&& other) = default;

        ~False() override = default;

        [[nodiscard]] std::string format() const override { return "false"; }

        [[nodiscard]] Type type() const override { return Type::False; }

        [[nodiscard]] bool is_truthy() const override { return false; }

        // TODO(piyush) Check for nullptr
        bool operator==(Data* rhs) override { return rhs->type() == Type::False; }
};

}  // namespace mal

#endif  // FALSE_HPP
