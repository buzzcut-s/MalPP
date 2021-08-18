#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>
#include <vector>

#include "../Data.hpp"

namespace mal
{

class Vector : public Data
{
public:
        Vector() = default;

        Vector(Vector const& other) = default;
        Vector& operator=(Vector const& other) = default;

        Vector(Vector&& other) = default;
        Vector& operator=(Vector&& other) = default;

        ~Vector() override = default;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override { return Type::Vector; }

        auto begin() { return m_vec.begin(); }
        auto end() { return m_vec.end(); }

        void push(mal::Data* value) { m_vec.push_back(value); }

        [[nodiscard]] size_t size() const { return m_vec.size(); }
        [[nodiscard]] bool   empty() const { return m_vec.empty(); }
        [[nodiscard]] auto   at(std::size_t idx) const { return m_vec.at(idx); }

private:
        std::vector<mal::Data*> m_vec;
};

}  // namespace mal

#endif  // VECTOR_HPP
