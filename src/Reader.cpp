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
        static const auto TOKEN_REGEX =
            std::regex(R"((~@|[\[\]{}()'`~^@]|"(?:\\.|[^\\"])*"?|;.*|[^\s\[\]{}('"`,;)]+))");

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

auto read_str(std::string input) -> mal::Data*
{
        auto reader = Reader(tokenize(std::move(input)));

        return read_form(reader);
}

auto read_form(Reader& reader) -> mal::Data*
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

        if (token.value() == "true")
                return read_true(reader);
        if (token.value() == "false")
                return read_false(reader);
        if (token.value() == "nil")
                return read_nil(reader);

        return read_atom(reader);
}

auto read_atom(Reader& reader) -> mal::Data*
{
        const auto token_val = reader.peek().value();
        reader.consume();

        if (token_val.front() == '"')
                return new (mal::String)(token_val);

        int int_val{};
        if (const auto [p, ec] =
                std::from_chars(token_val.data(), token_val.data() + token_val.size(),
                                int_val);
            ec == std::errc())
        {
                return new (mal::Integer)(int_val);
        }

        return new (mal::Symbol)(token_val);
}

auto read_list(Reader& reader) -> mal::Data*
{
        reader.consume();

        auto* list = new (mal::List)();
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

auto read_vector(Reader& reader) -> mal::Data*
{
        reader.consume();

        auto* vec = new (mal::Vector)();
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

auto read_hashmap(Reader& reader) -> mal::Data*
{
        reader.consume();

        auto* hashmap = new (mal::HashMap)();
        while (auto token = reader.peek())
        {
                if (token.value() == "}")
                {
                        reader.consume();
                        return hashmap;
                }

                auto* key = read_form(reader);

                token = reader.peek();
                if (token.value() == "}")
                {
                        // TODO(piyush) What do?
                        std::cerr << "hashmap without value\n";
                        reader.consume();
                        return nullptr;
                }

                auto* value = read_form(reader);
                hashmap->insert(key, value);
        }

        std::cerr << "unbalanced";
        return nullptr;
}

auto read_special_form(Reader& reader) -> mal::Data*
{
        const auto token = reader.peek();
        const char type  = token.value().front();
        reader.consume();

        auto* special_list = new (mal::List)();
        if (type == '\'')
                special_list->push(new (mal::Symbol)("quote"));
        else if (type == '`')
                special_list->push(new (mal::Symbol)("quasiquote"));
        else if (type == '~')
        {
                const bool unquoted = (token.value().length() == 1);
                if (unquoted)
                        special_list->push(new (mal::Symbol)("unquote"));
                else
                        special_list->push(new (mal::Symbol)("splice-unquote"));
        }
        else if (type == '@')
                special_list->push(new (mal::Symbol)("deref"));

        if (auto* val = read_form(reader); val)
        {
                special_list->push(val);
                return special_list;
        }

        std::cerr << "unbalanced";
        return nullptr;
}

auto read_with_meta(Reader& reader) -> mal::Data*
{
        reader.consume();

        auto* metadata = read_form(reader);
        auto* value    = read_form(reader);
        if (metadata && value)
        {
                auto* metadata_list = new (mal::List)();
                metadata_list->push(new (mal::Symbol)("with-meta"));
                metadata_list->push(value);
                metadata_list->push(metadata);
                return metadata_list;
        }

        std::cerr << "unbalanced";
        return nullptr;
}

auto read_true(Reader& reader) -> mal::Data*
{
        reader.consume();
        return new (mal::True)();
}

auto read_false(Reader& reader) -> mal::Data*
{
        reader.consume();
        return new (mal::False)();
}

auto read_nil(Reader& reader) -> mal::Data*
{
        reader.consume();
        return new (mal::Nil)();
}
