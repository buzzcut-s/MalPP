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

        [[nodiscard]] auto peek() const -> std::optional<std::string>;

        [[nodiscard]] auto next() -> std::optional<std::string>;

        void consume();

private:
        const std::vector<std::string>                 m_tokens;
        std::vector<std::string>::const_iterator       m_index;
        const std::vector<std::string>::const_iterator m_end;

        [[nodiscard]] bool eof() const { return m_index == m_end; }
};

auto tokenize(std::string input) -> std::vector<std::string>;

auto read_str(std::string input) -> mal::Data*;

auto read_form(Reader& reader) -> mal::Data*;

auto read_atom(Reader& reader) -> mal::Data*;

auto read_list(Reader& reader) -> mal::Data*;

auto read_vector(Reader& reader) -> mal::Data*;

auto read_hashmap(Reader& reader) -> mal::Data*;

auto read_special_form(Reader& reader) -> mal::Data*;

auto read_with_meta(Reader& reader) -> mal::Data*;

auto read_true(Reader& reader) -> mal::Data*;

auto read_false(Reader& reader) -> mal::Data*;

auto read_nil(Reader& reader) -> mal::Data*;

#endif  // READER_HPP
