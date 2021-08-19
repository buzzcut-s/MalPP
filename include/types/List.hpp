#ifndef LIST_HPP
#define LIST_HPP

#include <string>
#include <vector>

#include "../Data.hpp"

namespace mal
{

class List : public Data
{
public:
        List() = default;

        List(List const& other) = default;
        List& operator=(List const& other) = default;

        List(List&& other) = default;
        List& operator=(List&& other) = default;

        ~List() override = default;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override { return Type::List; }

        bool operator==(Data* rhs) override;

        auto begin() { return m_list.begin(); }
        auto end() { return m_list.end(); }

        auto front() { return m_list.front(); }
        auto data() { return m_list.data(); }

        void push(mal::Data* value) { m_list.push_back(value); }

        [[nodiscard]] size_t size() const { return m_list.size(); }
        [[nodiscard]] bool   empty() const { return m_list.empty(); }
        [[nodiscard]] auto   at(std::size_t idx) const -> mal::Data* { return m_list.at(idx); }

private:
        std::vector<mal::Data*> m_list;
};

}  // namespace mal

#endif  // LIST_HPP
