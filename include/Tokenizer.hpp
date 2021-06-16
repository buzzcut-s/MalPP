#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <optional>
#include <string_view>

class Tokenizer
{
public:
        explicit Tokenizer(std::string_view input);

        std::string_view                peek();
        std::optional<std::string_view> next();

private:
        std::string_view                 m_token;
        std::string_view::const_iterator m_index;
        std::string_view::const_iterator m_token_end;

        bool eof();
};

#endif  // TOKENIZER_HPP
