#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <utility>
#include <vector>

namespace mal
{

class Data
{
public:
        Data() = default;

        Data(Data const& other) = default;
        Data& operator=(Data const& other) = default;

        Data(Data&& other) = default;
        Data& operator=(Data&& other) = default;

        virtual ~Data() = default;

        [[nodiscard]] virtual std::string format() const = 0;
};

class Symbol : public Data
{
public:
        Symbol(Symbol const& other) = default;
        Symbol& operator=(Symbol const& other) = default;

        Symbol(Symbol&& other) = default;
        Symbol& operator=(Symbol&& other) = default;

        ~Symbol() override = default;

        explicit Symbol(std::string symbol) :
            m_symbol{std::move(symbol)}
        {}

        [[nodiscard]] std::string format() const override
        {
                return m_symbol;
        };

private:
        std::string m_symbol;
};

class List : public Data
{
public:
        List() = default;

        List(List const& other) = default;
        List& operator=(List const& other) = default;

        List(List&& other) = default;
        List& operator=(List&& other) = default;

        ~List() override = default;

        [[nodiscard]] std::string format() const override;

        void push(Data* value)
        {
                m_list.push_back(value);
        }

private:
        std::vector<mal::Data*> m_list;
};

}  // namespace mal

#endif  // TYPES_HPP
