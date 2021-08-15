#ifndef READER_HPP
#define READER_HPP

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace mal
{
class Data;
}

class Reader
{
public:
        explicit Reader(std::vector<std::string> tokens) :
            m_tokens(std::move(tokens)),
            m_index(m_tokens.cbegin()),
            m_end(m_tokens.cend())
        {
        }

        [[nodiscard]] auto peek() const -> std::optional<std::string>
        {
                if (!eof())
                        return *m_index;
                return std::nullopt;
        };

        auto next() -> std::optional<std::string>
        {
                if (!eof())
                        return *m_index++;
                return std::nullopt;
        };

        void consume()
        {
                if (!eof()) ++m_index;
        }

private:
        // TODO(piyush): Can this be a string_view?
        std::vector<std::string>                 m_tokens;
        std::vector<std::string>::const_iterator m_index;
        std::vector<std::string>::const_iterator m_end;

        [[nodiscard]] bool eof() const { return m_index == m_end; }
};

auto tokenize(std::string input) -> std::vector<std::string>;

auto read_str(std::string input) -> std::unique_ptr<mal::Data>;

auto read_form(Reader& reader) -> std::unique_ptr<mal::Data>;

auto read_atom(Reader& reader) -> std::unique_ptr<mal::Data>;

auto read_list(Reader& reader) -> std::unique_ptr<mal::Data>;

auto read_vector(Reader& reader) -> std::unique_ptr<mal::Data>;

auto read_hashmap(Reader& reader) -> std::unique_ptr<mal::Data>;

auto read_special_form(Reader& reader, char type) -> std::unique_ptr<mal::Data>;

auto read_with_meta(Reader& reader) -> std::unique_ptr<mal::Data>;

#endif  // READER_HPP
