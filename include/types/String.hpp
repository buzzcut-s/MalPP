#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <utility>

#include "../Data.hpp"

namespace mal
{

class String : public Data
{
public:
        String() = delete;

        explicit String(std::string string) :
            m_string{std::move(string)}
        {}

        String(String const& other) = default;
        String& operator=(String const& other) = default;

        String(String&& other) = default;
        String& operator=(String&& other) = default;

        ~String() override = default;

        [[nodiscard]] std::string format() const override { return m_string; }

        [[nodiscard]] Type type() const override { return Type::String; }

        bool operator==(Data* rhs) override;

        [[nodiscard]] std::string value() const { return m_string; }

private:
        std::string m_string;
};

}  // namespace mal

#endif  // STRING_HPP
