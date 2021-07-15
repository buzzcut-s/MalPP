#include "../include/Reader.hpp"

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <utility>

#include "../include/types.hpp"

auto tokenize(std::string input) -> std::vector<std::string>
{
        static const auto TOKEN_REGEX = std::regex(R"((~@|[\[\]{}()'`~^@]|"(?:\\.|[^\\"])*"?|;.*|[^\s\[\]{}('"`,;)]+))");

        std::string str{std::move(input)};
        std::smatch result{};

        std::vector<std::string> out;
        while (regex_search(str, result, TOKEN_REGEX))
        {
                if (result.str().front() == '"' && result.str().back() != '"')
                        std::cerr << "unbalanced\n";
                else
                        out.push_back(result.str());

                str = result.suffix();
        }

        return out;
}

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

        if (token.value()[0] == '(')
                return read_list(reader);

        if (token.value()[0] == '[')
                return read_vector(reader);

        return read_atom(reader);
}

auto read_atom(Reader& reader) -> mal::Data*
{
        auto* atom = new mal::Symbol{*reader.next()};
        return atom;
}

auto read_list(Reader& reader) -> mal::Data*
{
        reader.next();

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

        std::cerr << "unbalanced\n";
        return list;
}

auto read_vector(Reader& reader) -> mal::Data*
{
        reader.next();

        auto* vec = new mal::Vector;
        while (auto token = reader.peek())
        {
                if (*token == "]")
                {
                        reader.next();
                        return vec;
                }
                vec->push(read_form(reader));
        }

        std::cerr << "unbalanced\n";
        return vec;
}
