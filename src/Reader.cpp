#include "../include/Reader.hpp"

#include <iostream>
#include <memory>
#include <string_view>
#include <utility>

#include "../include/types.hpp"

Reader::Reader(std::vector<std::string> tokens) :
    m_tokens(std::move(tokens)),
    m_index(m_tokens.cbegin()),
    m_end(m_tokens.cend())
{
}

bool Reader::eof()
{
        return m_index == m_end;
}

std::optional<std::string> Reader::peek()
{
        if (!eof())
                return *m_index;

        return {};
}

std::optional<std::string> Reader::next()
{
        if (!eof())
                return *m_index++;

        return {};
}

std::vector<std::string> tokenize(std::string_view input)
{
        static const auto TOKEN_REGEX = std::regex(R"((~@|[\[\]{}()'`~^@]|"(?:\\.|[^\\"])*"?|;.*|[^\s\[\]{}('"`,;)]+))");

        std::string str{input};
        std::smatch result{};

        std::vector<std::string> out;
        while (regex_search(str, result, TOKEN_REGEX))
        {
                if ((result.str().front() == '"') && result.str().back() != '"')
                {
                        // TODO(piyush): Handle this
                        std::cout << "Mismatched quote\n";
                }
                else
                {
                        out.push_back(result.str());
                }

                str = result.suffix();
        }

        return out;
}

mal::Data* read_list(Reader& reader)
{
        reader.next();
        // TODO(piyush): What is this...?
        auto* list = new (mal::List);

        while (auto token = reader.peek())
        {
                if (*token == ")")
                {
                        reader.next();
                        break;
                }
                list->push(read_form(reader));
        }

        return list;
}

// TODO(piyush): Implement this
mal::Data* read_atom(Reader& reader)
{
        reader.next();

        return read_form(reader);
}

mal::Data* read_form(Reader& reader)
{
        auto token = reader.peek();

        if (!token)
                return nullptr;

        // TODO(piyush): Is this the right way?
        if (token.value()[0] == '(')
                return read_list(reader);

        return read_atom(reader);
}

// TODO(piyush): Order these definitions roughly by call order
mal::Data* read_str(std::string_view input)
{
        auto reader = Reader(tokenize(input));

        return read_form(reader);
}
