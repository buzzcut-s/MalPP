#ifndef KEYWORD_HPP
#define KEYWORD_HPP

#include <string>
#include <utility>

#include "../Data.hpp"

namespace mal
{

class Keyword : public Data
{
public:
        Keyword() = delete;

        explicit Keyword(std::string string) :
            m_keyword{std::move(string)}
        {}

        Keyword(Keyword const& other) = default;
        Keyword& operator=(Keyword const& other) = default;

        Keyword(Keyword&& other) = default;
        Keyword& operator=(Keyword&& other) = default;

        ~Keyword() override = default;

        [[nodiscard]] std::string format() const override { return m_keyword; }

        [[nodiscard]] Type type() const override { return Type::Keyword; }

        bool operator==(Data* rhs) override;

        [[nodiscard]] std::string value() const { return m_keyword; }

private:
        std::string m_keyword;
};

}  // namespace mal

#endif  // KEYWORD_HPP
