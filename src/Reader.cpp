#include "../include/Reader.hpp"

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <utility>

#include "../include/types.hpp"

Reader::Reader(std::vector<std::string> tokens) :
    m_tokens(std::move(tokens)),
    m_index(m_tokens.cbegin()),
    m_end(m_tokens.cend())
{
}

auto Reader::peek() const -> std::optional<std::string>
{
        if (!eof())
                return *m_index;

        return {};
}

auto Reader::next() -> std::optional<std::string>
{
        if (!eof())
                return *m_index++;

        return {};
}

bool Reader::eof() const
{
        return m_index == m_end;
}

auto tokenize(std::string input) -> std::vector<std::string>
{
        static const auto TOKEN_REGEX = std::regex(R"((~@|[\[\]{}()'`~^@]|"(?:\\.|[^\\"])*"?|;.*|[^\s\[\]{}('"`,;)]+))");

        std::string str{std::move(input)};
        std::smatch result{};

        std::vector<std::string> out;
        while (regex_search(str, result, TOKEN_REGEX))
        {
                if ((result.str().front() == '"') && result.str().back() != '"')
                        std::cout << "EOF\n";
                else
                        out.push_back(result.str());

                str = result.suffix();
        }

        return out;
}

// TODO(piyush): Order these definitions roughly by call order
auto read_str(std::string input) -> mal::Data*
{
        auto reader = Reader(tokenize(std::move(input)));

        return read_form(reader);
}

auto read_form(Reader& reader) -> mal::Data*
{
        auto token = reader.peek();

        if (!token)
                return nullptr;

        // TODO(piyush): Is this the right way?
        if (token.value()[0] == '(')
                return read_list(reader);

        return read_atom(reader);
}

// TODO(piyush): Implement this
auto read_atom(Reader& reader) -> mal::Data*
{
        return new mal::Symbol{*reader.next()};
}

auto read_list(Reader& reader) -> mal::Data*
{
        reader.next();

        // TODO(piyush): What is this...?
        auto* list = new mal::List;

        while (auto token = reader.peek())
        {
                if (*token == ")")
                {
                        reader.next();
                        return list;
                }
                list->push(read_form(reader));
        }

        std::cerr << "EOF\n";
        return list;
}
