#include "../include/Reader.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "../include/types.hpp"

Reader::Reader(std::vector<std::string> tokens) :
    m_tokens(std::move(tokens)),
    m_index(m_tokens.cbegin()),
    m_end(m_tokens.cend())
{
}

bool Reader::eof() const
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

void Reader::print()
{
        int i = 0;
        for (auto it = m_index; it < m_end; ++it)
        {
                std::cerr << i++ << " " << *it << "\n";
        }
}

std::vector<std::string> tokenize(std::string input)
{
        static const auto TOKEN_REGEX = std::regex(R"((~@|[\[\]{}()'`~^@]|"(?:\\.|[^\\"])*"?|;.*|[^\s\[\]{}('"`,;)]+))");

        std::string str{std::move(input)};
        std::smatch result{};

        std::vector<std::string> out;
        while (regex_search(str, result, TOKEN_REGEX))
        {
                if ((result.str().front() == '"') && result.str().back() != '"')
                {
                        // TODO(piyush): Handle this
                        std::cout << "EOF\n";
                }
                else
                {
                        out.push_back(result.str());
                }

                str = result.suffix();
        }

        return out;
}

// TODO(piyush): Order these definitions roughly by call order
mal::Data* read_str(std::string input)
{
        auto reader = Reader(tokenize(std::move(input)));

        // reader.print();

        return read_form(reader);
}

mal::Data* read_form(Reader& reader)
{
        auto token = reader.peek();

        if (!token)
                return nullptr;

        // std::cerr << "-" << *token << "\n";

        // TODO(piyush): Is this the right way?
        if (token.value()[0] == '(')
                return read_list(reader);

        return read_atom(reader);
}

mal::Data* read_list(Reader& reader)
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

// TODO(piyush): Implement this
mal::Data* read_atom(Reader& reader)
{
        return new mal::Symbol{*reader.next()};
}