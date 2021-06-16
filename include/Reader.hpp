#ifndef READER_HPP
#define READER_HPP

#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#include "types.hpp"

class Reader
{
public:
        explicit Reader(std::vector<std::string> tokens);

        std::optional<std::string> peek();
        std::optional<std::string> next();

private:
        std::vector<std::string>                 m_tokens;
        std::vector<std::string>::const_iterator m_index;
        std::vector<std::string>::const_iterator m_end;

        bool eof();
};

std::vector<std::string> tokenize(std::string_view input);

mal::Data* read_form(Reader& reader);

mal::Data* read_list(Reader& reader);

mal::Data* read_atom(Reader& reader);

mal::Data* read_str(std::string_view input);

#endif  // READER_HPP
