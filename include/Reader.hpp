#ifndef READER_HPP
#define READER_HPP

#include <optional>
#include <string>
#include <vector>

namespace mal
{
class Data;
}

class Reader
{
public:
        explicit Reader(std::vector<std::string> tokens);

        // TODO(piyush): Is optional really required? Although it's a nice to have
        [[nodiscard]] auto peek() const -> std::optional<std::string>;
        auto               next() -> std::optional<std::string>;

private:
        // TODO(piyush): Can this be a string_view?
        std::vector<std::string>                 m_tokens;
        std::vector<std::string>::const_iterator m_index;
        std::vector<std::string>::const_iterator m_end;

        [[nodiscard]] bool eof() const;
};

auto tokenize(std::string input) -> std::vector<std::string>;

auto read_str(std::string input) -> mal::Data*;

auto read_form(Reader& reader) -> mal::Data*;

auto read_atom(Reader& reader) -> mal::Data*;

auto read_list(Reader& reader) -> mal::Data*;

#endif  // READER_HPP
