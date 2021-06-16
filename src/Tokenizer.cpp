#include "../include/Tokenizer.hpp"

#include <iostream>
#include <iterator>
#include <optional>
#include <string_view>

using sview = std::string_view;

Tokenizer::Tokenizer(std::string_view input) :
    m_token(input),
    m_index(input.cbegin()),
    m_token_end(input.cend())
{
}

std::string_view Tokenizer::peek()
{
        return m_token;
}

std::optional<std::string_view> Tokenizer::next()
{
        while (!eof())
        {
                switch (*m_index)
                {
                        case ' ':
                        case '\t':
                        case '\n':
                        case ',':
                                ++m_index;
                                break;

                        case '~':
                        {
                                ++m_index;
                                if (!eof() && *m_index == '@')
                                {
                                        ++m_index;
                                        return sview(m_index - 2, 2);
                                }
                                return sview(m_index - 1, 1);
                        }
                        case '[':
                        case ']':
                        case '{':
                        case '}':
                        case '(':
                        case ')':
                        case '\'':
                        case '`':
                        case '^':
                        case '@':
                                return sview(m_index++, 1);
                        case '"':
                        {
                                const auto* start = m_index;
                                ++m_index;
                                while (!eof())
                                {
                                        switch (*m_index)
                                        {
                                                case '"':
                                                        ++m_index;
                                                        return sview(start, m_index - start);
                                                case '\\':
                                                        ++m_index;
                                                        break;
                                        }
                                        ++m_index;
                                }
                                return "EOF";
                        }
                        case ';':
                        {
                                const auto* start = m_index;
                                while (!eof())
                                {
                                        if (*m_index == '\n')
                                                break;
                                        ++m_index;
                                }
                                return sview(start, m_index - start);
                        }

                        default:
                        {
                                const auto* start = m_index;
                                bool        done  = false;
                                while (!done && !eof())
                                {
                                        switch (*m_index)
                                        {
                                                case ' ':
                                                case '\t':
                                                case '\n':
                                                case '[':
                                                case ']':
                                                case '{':
                                                case '}':
                                                case '(':
                                                case ')':
                                                case '\'':
                                                case '"':
                                                case '`':
                                                case ',':
                                                case ';':
                                                        done = true;
                                                        break;
                                                default:
                                                        ++m_index;
                                        }
                                }
                                return sview(start, m_index - start);
                        }
                }
        }

        return {};
}

bool Tokenizer::eof()
{
        return m_index == m_token_end;
}
