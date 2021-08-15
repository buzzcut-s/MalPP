#include "../include/Reader.hpp"

#include <algorithm>
#include <charconv>
#include <iostream>
#include <memory>
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
        while (std::regex_search(str, result, TOKEN_REGEX))
        {
                const auto token = result.str();

                // TODO(piyush) Make this more sensible
                if (token.front() == '"')
                {
                        // TODO(piyush) Can easily reduce scope of this std::count().
                        auto escape_count = std::count(token.cbegin(), token.cend(), '\\');
                        if (token.size() == 1 || token.back() != '"'
                            || (token[token.size() - 2] == '\\' && escape_count % 2 != 0))
                        {
                                std::cerr << "unbalanced";
                        }
                        else
                        {
                                out.emplace_back(result.str());
                        }
                }
                else
                {
                        out.emplace_back(result.str());
                }

                str = result.suffix();
        }

        return out;
}

auto read_str(std::string input) -> mal::DataPtr
{
        auto reader = Reader(tokenize(std::move(input)));

        return read_form(reader);
}

auto read_form(Reader& reader) -> mal::DataPtr
{
        const auto token = reader.peek();

        if (!token)
                return nullptr;

        const auto type = token.value().front();

        if (type == '(')
                return read_list(reader);
        if (type == '[')
                return read_vector(reader);
        if (type == '{')
                return read_hashmap(reader);
        if (type == '\'' || type == '`' || type == '~' || type == '@')
                return read_special_form(reader);
        if (type == '^')
                return read_with_meta(reader);

        return read_atom(reader);
}

auto read_atom(Reader& reader) -> mal::DataPtr
{
        const auto token_val = reader.peek().value();
        reader.consume();

        if (token_val.front() == '"')
                return std::make_unique<mal::String>(token_val);

        int int_val{};
        if (const auto [p, ec] =
                std::from_chars(token_val.data(), token_val.data() + token_val.size(),
                                int_val);
            ec == std::errc())
        {
                return std::make_unique<mal::Integer>(int_val);
        }

        return std::make_unique<mal::Symbol>(token_val);
}

auto read_list(Reader& reader) -> mal::DataPtr
{
        reader.consume();

        auto list = std::make_unique<mal::List>();
        while (auto token = reader.peek())
        {
                if (token.value() == ")")
                {
                        reader.consume();
                        return list;
                }
                list->push(read_form(reader));
        }

        std::cerr << "unbalanced";
        return nullptr;
}

auto read_vector(Reader& reader) -> mal::DataPtr
{
        reader.consume();

        auto vec = std::make_unique<mal::Vector>();
        while (auto token = reader.peek())
        {
                if (token.value() == "]")
                {
                        reader.consume();
                        return vec;
                }
                vec->push(read_form(reader));
        }

        std::cerr << "unbalanced";
        return nullptr;
}

auto read_hashmap(Reader& reader) -> mal::DataPtr
{
        reader.consume();

        auto hashmap = std::make_unique<mal::HashMap>();
        while (auto token = reader.peek())
        {
                if (token.value() == "}")
                {
                        reader.consume();
                        return hashmap;
                }

                auto key = read_form(reader);

                token = reader.peek();
                if (token.value() == "}")
                {
                        // TODO(piyush) What do?
                        std::cerr << "hashmap without value\n";
                        reader.consume();
                        return nullptr;
                }

                auto value = read_form(reader);
                hashmap->insert(std::move(key), std::move(value));
        }

        std::cerr << "unbalanced";
        return nullptr;
}

auto read_special_form(Reader& reader) -> mal::DataPtr
{
        const auto token = reader.peek();
        const char type  = token.value().front();
        reader.consume();

        auto special_list = std::make_unique<mal::List>();
        if (type == '\'')
                special_list->push(std::make_unique<mal::Symbol>("quote"));
        else if (type == '`')
                special_list->push(std::make_unique<mal::Symbol>("quasiquote"));
        else if (type == '~')
        {
                const bool unquoted = (token.value().length() == 1);
                if (unquoted)
                        special_list->push(std::make_unique<mal::Symbol>("unquote"));
                else
                        special_list->push(std::make_unique<mal::Symbol>("splice-unquote"));
        }
        else if (type == '@')
                special_list->push(std::make_unique<mal::Symbol>("deref"));

        if (auto val = read_form(reader); val)
        {
                special_list->push(std::move(val));
                return special_list;
        }

        std::cerr << "unbalanced";
        return nullptr;
}

auto read_with_meta(Reader& reader) -> mal::DataPtr
{
        reader.consume();

        auto metadata = read_form(reader);
        auto value    = read_form(reader);
        if (metadata && value)
        {
                auto metadata_list = std::make_unique<mal::List>();
                metadata_list->push(std::make_unique<mal::Symbol>("with-meta"));
                metadata_list->push(std::move(value));
                metadata_list->push(std::move(metadata));
                return metadata_list;
        }

        std::cerr << "unbalanced";
        return nullptr;
}
