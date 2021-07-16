#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <unordered_map>
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

        [[nodiscard]] size_t size() const
        {
                return m_list.size();
        }

private:
        std::vector<mal::Data*> m_list;
};

class Vector : public Data
{
public:
        Vector() = default;

        Vector(Vector const& other) = default;
        Vector& operator=(Vector const& other) = default;

        Vector(Vector&& other) = default;
        Vector& operator=(Vector&& other) = default;

        ~Vector() override = default;

        [[nodiscard]] std::string format() const override;

        void push(Data* value)
        {
                m_vector.push_back(value);
        }

private:
        std::vector<mal::Data*> m_vector;
};

class HashMap : public Data
{
public:
        HashMap() = default;

        HashMap(HashMap const& other) = default;
        HashMap& operator=(HashMap const& other) = default;

        HashMap(HashMap&& other) = default;
        HashMap& operator=(HashMap&& other) = default;

        ~HashMap() override = default;

        [[nodiscard]] std::string format() const override;

        void insert(mal::Data* key, mal::Data* value)
        {
                m_hashmap[key] = value;
        }

        [[nodiscard]] mal::Data* find(mal::Data* key) const
        {
                if (auto res = m_hashmap.find(key); res != m_hashmap.end())
                        return res->second;
                return nullptr;
        }

protected:
        struct DataHasher
        {
                std::size_t operator()(const mal::Data* key) const noexcept
                {
                        return std::hash<std::string>{}(key->format());
                }
        };

        struct DataPred
        {
                // TODO(piyush) Implement this, for real (equality)
                constexpr bool operator()(const mal::Data* lhs, const mal::Data* rhs) const
                {
                        return lhs == rhs;
                }
        };

private:
        std::unordered_map<mal::Data*, mal::Data*, DataHasher, DataPred> m_hashmap;
};

}  // namespace mal

#endif  // TYPES_HPP
