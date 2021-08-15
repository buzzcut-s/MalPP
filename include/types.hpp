#ifndef TYPES_HPP
#define TYPES_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace mal
{

class Data
{
        // Can't implement an evaluate() pure virtual here because we want to recursively call EVAL
        // Pretty sure we'll have to do shitty stuff to return actual types in eval_ast...
        // static_cast imminent

public:
        Data() = default;

        Data(Data const& other) = default;
        Data& operator=(Data const& other) = default;

        Data(Data&& other) = default;
        Data& operator=(Data&& other) = default;

        virtual ~Data() = default;

        [[nodiscard]] virtual std::string format() const = 0;
};

using DataPtr = std::unique_ptr<mal::Data>;

class Integer : public Data
{
public:
        Integer(Integer const& other) = default;
        Integer& operator=(Integer const& other) = default;

        Integer(Integer&& other) = default;
        Integer& operator=(Integer&& other) = default;

        ~Integer() override = default;

        explicit Integer(int int_value) :
            m_int{int_value}
        {}

        // TODO(piyush) This does look pretty nice, is it?
        int operator+(const Integer& rhs) const
        {
                return m_int + rhs.m_int;
        }

        int operator-(const Integer& rhs) const
        {
                return m_int - rhs.m_int;
        }

        int operator*(const Integer& rhs) const
        {
                return m_int * rhs.m_int;
        }

        int operator/(const Integer& rhs) const
        {
                return m_int / rhs.m_int;
        }

        [[nodiscard]] std::string format() const override
        {
                return std::to_string(m_int);
        }

private:
        int m_int;
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
        }

private:
        // TODO(piyush) char?
        std::string m_symbol;
};

class String : public Data
{
public:
        String(String const& other) = default;
        String& operator=(String const& other) = default;

        String(String&& other) = default;
        String& operator=(String&& other) = default;

        ~String() override = default;

        explicit String(std::string symbol) :
            m_string{std::move(symbol)}
        {}

        [[nodiscard]] std::string format() const override
        {
                return m_string;
        }

private:
        std::string m_string;
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

        void push(DataPtr value)
        {
                m_list.push_back(std::move(value));
        }

        [[nodiscard]] size_t size() const
        {
                return m_list.size();
        }

private:
        std::vector<DataPtr> m_list;
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

        void push(DataPtr value)
        {
                m_vector.push_back(std::move(value));
        }

private:
        std::vector<DataPtr> m_vector;
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

        void insert(DataPtr key, DataPtr value)
        {
                m_hashmap.emplace(std::move(key), std::move(value));
        }

        [[nodiscard]] mal::Data* find(DataPtr key) const
        {
                if (auto res = m_hashmap.find(key); res != m_hashmap.cend())
                        return res->second.get();
                return nullptr;
        }

protected:
        // TODO(piyush) Can use these objects for our symbol : lambda map
        struct DataHasher
        {
                std::size_t operator()(const DataPtr& key) const noexcept
                {
                        return std::hash<std::string>{}(key->format());
                }
        };

        struct DataPred
        {
                // TODO(piyush) Implement this, for real (equality)
                bool operator()(const DataPtr& lhs, const DataPtr& rhs) const
                {
                        // TODO(piyush) Changed this to check with string values. Ok?
                        return lhs->format() == rhs->format();
                }
        };

private:
        std::unordered_map<DataPtr, DataPtr, DataHasher, DataPred> m_hashmap;
};

}  // namespace mal

#endif  // TYPES_HPP
