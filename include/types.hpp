#ifndef TYPES_HPP
#define TYPES_HPP

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace mal
{

class Integer;
class Symbol;
class List;
class EvalList;
class Function;

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

        enum class Type
        {
                Integer,
                Symbol,
                String,
                List,
                Vector,
                HashMap,
                EvalList,
                Function
        };

        [[nodiscard]] virtual Type type() const = 0;

        Integer*  integer();
        Symbol*   symbol();
        List*     list();
        EvalList* eval_list();
        Function* function();
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

        [[nodiscard]] Type type() const override
        {
                return Type::Integer;
        }

        [[nodiscard]] int value() const
        {
                return m_int;
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

        [[nodiscard]] Type type() const override
        {
                return Type::Symbol;
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

        [[nodiscard]] Type type() const override
        {
                return Type::String;
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

        auto begin() { return m_list.begin(); }
        auto end() { return m_list.end(); }

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override
        {
                return Type::List;
        }

        void push(DataPtr value)
        {
                m_list.push_back(std::move(value));
        }

        [[nodiscard]] size_t size() const
        {
                return m_list.size();
        }

        [[nodiscard]] bool empty() const
        {
                return m_list.empty();
        }

        [[nodiscard]] mal::Data* at(std::size_t idx) const
        {
                return m_list.at(idx).get();
        }

        [[nodiscard]] auto data() const
        {
                return m_list.data();
        }

private:
        std::vector<DataPtr> m_list;
};

class EvalList : public Data
{
public:
        EvalList() = default;

        EvalList(EvalList const& other) = default;
        EvalList& operator=(EvalList const& other) = default;

        EvalList(EvalList&& other) = default;
        EvalList& operator=(EvalList&& other) = default;

        ~EvalList() override = default;

        auto begin() { return m_eval_list.begin(); }
        auto end() { return m_eval_list.end(); }

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override
        {
                return Type::EvalList;
        }

        void push(Data* value)
        {
                m_eval_list.push_back(value);
        }

        [[nodiscard]] size_t size() const
        {
                return m_eval_list.size();
        }

        [[nodiscard]] mal::Data* at(std::size_t idx) const
        {
                return m_eval_list.at(idx);
        }

        [[nodiscard]] auto data() const
        {
                return m_eval_list.data();
        }

private:
        std::vector<Data*> m_eval_list;
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

        [[nodiscard]] Type type() const override
        {
                return Type::Vector;
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

        [[nodiscard]] auto find(const DataPtr& key) const -> mal::Data*
        {
                if (auto res = m_hashmap.find(key); res != m_hashmap.cend())
                        return res->second.get();
                return nullptr;
        }

        [[nodiscard]] Type type() const override
        {
                return Type::HashMap;
        }

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

class Function : public Data
{
public:
        using Fn = std::function<mal::Data*(std::size_t argc, mal::Data* const* args)>;

        Function() = default;

        explicit Function(Fn fn) :
            m_fn{std::move(fn)}
        {}

        Function(Function const& other) = default;
        Function& operator=(Function const& other) = default;

        Function(Function&& other) = default;
        Function& operator=(Function&& other) = default;

        ~Function() override = default;

        [[nodiscard]] std::string format() const override { return ""; }

        [[nodiscard]] Type type() const override
        {
                return Type::Function;
        }

        [[nodiscard]] auto value() const
        {
                return m_fn;
        }

        mal::Data* apply(size_t argc, mal::Data* const* args)
        {
                return m_fn(argc, args);
        }

private:
        Fn m_fn{};
};

}  // namespace mal

#endif  // TYPES_HPP
