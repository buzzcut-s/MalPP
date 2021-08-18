#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include <utility>

#include "../Data.hpp"

namespace mal
{

class Symbol : public Data
{
public:
        Symbol() = delete;

        explicit Symbol(std::string symbol) :
            m_symbol{std::move(symbol)}
        {}

        Symbol(Symbol const& other) = default;
        Symbol& operator=(Symbol const& other) = default;

        Symbol(Symbol&& other) = default;
        Symbol& operator=(Symbol&& other) = default;

        ~Symbol() override = default;

        [[nodiscard]] std::string format() const override { return m_symbol; }

        [[nodiscard]] Type type() const override { return Type::Symbol; }

        [[nodiscard]] std::string value() const { return m_symbol; }

        [[nodiscard]] bool is_def() const { return m_symbol == "def!"; }
        [[nodiscard]] bool is_let() const { return m_symbol == "let*"; }
        [[nodiscard]] bool is_do() const { return m_symbol == "do"; }
        [[nodiscard]] bool is_if() const { return m_symbol == "if"; }
        [[nodiscard]] bool is_fn() const { return m_symbol == "fn*"; }
        [[nodiscard]] bool is_variadic() const { return m_symbol == "&"; }

private:
        std::string m_symbol;
};

}  // namespace mal

#endif  // SYMBOL_HPP
