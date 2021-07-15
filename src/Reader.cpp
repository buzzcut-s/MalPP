#include "../include/Reader.hpp"

#include <algorithm>
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
                auto token = result.str();

                // TODO(piyush) Make this more sensible
                if (token.front() == '"')
                {
                        auto escape_count = std::count(token.cbegin(), token.cend(), '\\');
                        if (token.size() == 1 || token.back() != '"'
                            || (token[token.size() - 2] == '\\' && escape_count % 2 != 0))
                        {
                                std::cerr << "unbalanced";
                        }
                        else
                        {
                                out.push_back(result.str());
                        }
                }
                else
                {
                        out.push_back(result.str());
                }

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

        auto type = token.value()[0];

        if (type == '(')
                return read_list(reader);
        if (type == '[')
                return read_vector(reader);
        if (type == '{')
                return read_hashmap(reader);
        if (type == '\'' || type == '`' || type == '~' || type == '@')
                return read_special_form(reader, type);

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

        std::cerr << "unbalanced";
        return nullptr;
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

        std::cerr << "unbalanced";
        return nullptr;
}

auto read_hashmap(Reader& reader) -> mal::Data*
{
        reader.next();

        auto* hashmap = new mal::HashMap;
        while (auto token = reader.peek())
        {
                if (*token == "}")
                {
                        reader.next();
                        return hashmap;
                }

                auto* key = read_form(reader);

                token = reader.peek();
                if (*token == "}")
                {
                        // TODO(piyush) What do?
                        std::cerr << "hashmap without value\n";
                        reader.next();
                        return nullptr;
                }

                auto* value = read_form(reader);
                hashmap->insert(key, value);
        }

        std::cerr << "unbalanced";
        return nullptr;
}

auto read_special_form(Reader& reader, char type) -> mal::Data*
{
        auto* special_list = new mal::List;

        reader.next();

        if (type == '\'')
                special_list->push(new mal::Symbol{"quote"});
        else if (type == '`')
                special_list->push(new mal::Symbol{"quasiquote"});
        else if (type == '~')
                special_list->push(new mal::Symbol{"unquote"});
        else if (type == '@')
                special_list->push(new mal::Symbol{"deref"});

        if (auto* val = read_form(reader); val)
        {
                special_list->push(val);
                return special_list;
        }

        std::cerr << "unbalanced";
        return nullptr;
}