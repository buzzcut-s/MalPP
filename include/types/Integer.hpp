#ifndef INTEGER_HPP
#define INTEGER_HPP

#include <string>

#include "../Data.hpp"

namespace mal
{

class Integer : public Data
{
public:
        Integer() = delete;

        explicit Integer(int int_value) :
            m_int{int_value}
        {}

        Integer(Integer const& other) = default;
        Integer& operator=(Integer const& other) = default;

        Integer(Integer&& other) = default;
        Integer& operator=(Integer&& other) = default;

        ~Integer() override = default;

        [[nodiscard]] std::string format() const override { return std::to_string(m_int); }

        [[nodiscard]] Type type() const override { return Type::Integer; }

        bool operator==(Data* rhs) override;

        int operator+(const Integer& rhs) const { return m_int + rhs.m_int; }
        int operator-(const Integer& rhs) const { return m_int - rhs.m_int; }
        int operator*(const Integer& rhs) const { return m_int * rhs.m_int; }
        int operator/(const Integer& rhs) const { return m_int / rhs.m_int; }

        [[nodiscard]] int value() const { return m_int; }

private:
        int m_int;
};

}  // namespace mal

#endif  // INTEGER_HPP
