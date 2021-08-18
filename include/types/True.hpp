#ifndef TRUE_HPP
#define TRUE_HPP

#include <string>

#include "../Data.hpp"

namespace mal
{

class True : public Data
{
public:
        True() = default;

        True(True const& other) = default;
        True& operator=(True const& other) = default;

        True(True&& other) = default;
        True& operator=(True&& other) = default;

        ~True() override = default;

        [[nodiscard]] std::string format() const override { return "true"; }

        [[nodiscard]] Type type() const override { return Type::True; }

        bool operator==(Data* rhs) override { return rhs->type() == Type::True; }
};

}  // namespace mal

#endif  // TRUE_HPP
