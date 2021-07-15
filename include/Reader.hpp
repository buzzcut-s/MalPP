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

        // TODO(piyush): Is optional really required? Although it's a nice to have
        std::optional<std::string> peek();
        std::optional<std::string> next();

        void print();

private:
        // TODO(piyush): Can this be a string_view?
        std::vector<std::string>                 m_tokens;
        std::vector<std::string>::const_iterator m_index;
        std::vector<std::string>::const_iterator m_end;

        bool eof() const;
};

std::vector<std::string> tokenize(std::string input);

mal::Data* read_form(Reader& reader);

mal::Data* read_list(Reader& reader);

mal::Data* read_atom(Reader& reader);

mal::Data* read_str(std::string input);

#endif  // READER_HPP
