#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace mal
{

class Data
{
public:
        virtual ~Data() = default;

        virtual std::string format() = 0;
};

class List : public Data
{
public:
        ~List() override = default;

        std::string format() override;

        void push(Data* value)
        {
                m_list.push_back(value);
        }

private:
        std::vector<mal::Data*> m_list;
};

class Symbol : public Data
{
public:
        explicit Symbol(std::string symbol) :
            m_symbol{std::move(symbol)}
        {}

        ~Symbol() override = default;

        std::string format() override;

        std::string symbol() const { return m_symbol; }

private:
        std::string m_symbol;
};

}  // namespace mal
#endif  // TYPES_HPP
