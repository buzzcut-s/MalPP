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

class Symbol;
class List;
class Vector;
class HashMap;
class Integer;
class String;
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
                Symbol,
                List,
                Vector,
                HashMap,
                Integer,
                String,
                True,
                False,
                Nil,
                Function,
        };

        [[nodiscard]] virtual Type type() const = 0;

        [[nodiscard]] virtual bool is_truthy() const { return true; }

        virtual bool operator==(Data* /*rhs*/) { return false; }
        virtual bool operator!=(Data* rhs) { return !(*this == rhs); }

        Symbol*   symbol();
        List*     list();
        Vector*   vector();
        HashMap*  hashmap();
        Integer*  integer();
        String*   string();
        Function* function();
};

class Integer : public Data
{
public:
        Integer() = delete;

        explicit Integer(int int_value) :
            m_int{int_value}
        {}

        Integer(Integer const& other) = default;
        Integer& operator=(Integer const& other) = default;

        Integer(Integer&& other) = default;
        Integer& operator=(Integer&& other) = default;

        ~Integer() override = default;

        [[nodiscard]] std::string format() const override { return std::to_string(m_int); }

        [[nodiscard]] Type type() const override { return Type::Integer; }

        bool operator==(Data* rhs) override;

        int operator+(const Integer& rhs) const { return m_int + rhs.m_int; }
        int operator-(const Integer& rhs) const { return m_int - rhs.m_int; }
        int operator*(const Integer& rhs) const { return m_int * rhs.m_int; }
        int operator/(const Integer& rhs) const { return m_int / rhs.m_int; }

        [[nodiscard]] int value() const { return m_int; }

private:
        int m_int;
};

class Symbol : public Data
{
public:
        Symbol() = delete;

        explicit Symbol(std::string symbol) :
            m_symbol{std::move(symbol)}
        {}

        Symbol(Symbol const& other) = default;
        Symbol& operator=(Symbol const& other) = default;

        Symbol(Symbol&& other) = default;
        Symbol& operator=(Symbol&& other) = default;

        ~Symbol() override = default;

        [[nodiscard]] std::string format() const override { return m_symbol; }

        [[nodiscard]] Type type() const override { return Type::Symbol; }

        [[nodiscard]] std::string value() const { return m_symbol; }

        [[nodiscard]] bool is_def() const { return m_symbol == "def!"; }
        [[nodiscard]] bool is_let() const { return m_symbol == "let*"; }
        [[nodiscard]] bool is_do() const { return m_symbol == "do"; }
        [[nodiscard]] bool is_if() const { return m_symbol == "if"; }
        [[nodiscard]] bool is_fn() const { return m_symbol == "fn*"; }

private:
        std::string m_symbol;
};

class String : public Data
{
public:
        String() = delete;

        explicit String(std::string string) :
            m_string{std::move(string)}
        {}

        String(String const& other) = default;
        String& operator=(String const& other) = default;

        String(String&& other) = default;
        String& operator=(String&& other) = default;

        ~String() override = default;

        [[nodiscard]] std::string format() const override { return m_string; }

        [[nodiscard]] Type type() const override { return Type::String; }

        bool operator==(Data* rhs) override;

        [[nodiscard]] std::string value() const { return m_string; }

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

        [[nodiscard]] Type type() const override { return Type::List; }

        bool operator==(Data* rhs) override;

        auto begin() { return m_list.begin(); }
        auto end() { return m_list.end(); }

        auto front() { return m_list.front(); }
        auto data() { return m_list.data(); }

        void push(mal::Data* value) { m_list.push_back(value); }

        [[nodiscard]] size_t size() const { return m_list.size(); }
        [[nodiscard]] bool   empty() const { return m_list.empty(); }
        [[nodiscard]] auto   at(std::size_t idx) const { return m_list.at(idx); }

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

        [[nodiscard]] Type type() const override { return Type::Vector; }

        auto begin() { return m_vec.begin(); }
        auto end() { return m_vec.end(); }

        void push(mal::Data* value) { m_vec.push_back(value); }

        [[nodiscard]] size_t size() const { return m_vec.size(); }
        [[nodiscard]] bool   empty() const { return m_vec.empty(); }
        [[nodiscard]] auto   at(std::size_t idx) const { return m_vec.at(idx); }

private:
        std::vector<mal::Data*> m_vec;
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

        [[nodiscard]] Type type() const override { return Type::HashMap; }

        bool operator==(Data* rhs) override;

        auto begin() { return m_hashmap.begin(); }
        auto end() { return m_hashmap.end(); }

        [[nodiscard]] size_t size() const { return m_hashmap.size(); }

        void insert(mal::Data* key, mal::Data* value) { m_hashmap.emplace(key, value); }

        [[nodiscard]] auto find(mal::Data* key) const -> mal::Data*;

private:
        struct DataHasher
        {
                std::size_t operator()(const mal::Data* key) const noexcept;
        };

        struct DataPred
        {
                bool operator()(const mal::Data* lhs, const mal::Data* rhs) const;
        };

        std::unordered_map<mal::Data*, mal::Data*, DataHasher, DataPred> m_hashmap;
};

class True : public Data
{
public:
        True() = default;

        True(True const& other) = default;
        True& operator=(True const& other) = default;

        True(True&& other) = default;
        True& operator=(True&& other) = default;

        ~True() override = default;

        [[nodiscard]] std::string format() const override { return "true"; }

        [[nodiscard]] Type type() const override { return Type::True; }

        bool operator==(Data* rhs) override { return rhs->type() == Type::True; }
};

class False : public Data
{
public:
        False() = default;

        False(False const& other) = default;
        False& operator=(False const& other) = default;

        False(False&& other) = default;
        False& operator=(False&& other) = default;

        ~False() override = default;

        [[nodiscard]] std::string format() const override { return "false"; }

        [[nodiscard]] Type type() const override { return Type::False; }

        [[nodiscard]] bool is_truthy() const override { return false; }

        bool operator==(Data* rhs) override { return rhs->type() == Type::False; }
};

class Nil : public Data
{
public:
        Nil() = default;

        Nil(Nil const& other) = default;
        Nil& operator=(Nil const& other) = default;

        Nil(Nil&& other) = default;
        Nil& operator=(Nil&& other) = default;

        ~Nil() override = default;

        [[nodiscard]] std::string format() const override { return "nil"; }

        [[nodiscard]] Type type() const override { return Type::Nil; }

        [[nodiscard]] bool is_truthy() const override { return false; }

        bool operator==(Data* rhs) override { return rhs->type() == Type::Nil; }
};

class Function : public Data
{
public:
        using Fn = std::function<mal::Data*(const std::size_t argc, mal::Data* const* args)>;

        Function() = delete;

        explicit Function(Fn fn) :
            m_fn{std::move(fn)}
        {}

        Function(Function const& other) = default;
        Function& operator=(Function const& other) = delete;

        Function(Function&& other) = default;
        Function& operator=(Function&& other) = delete;

        ~Function() override = default;

        [[nodiscard]] std::string format() const override { return "#<function>"; }

        [[nodiscard]] Type type() const override { return Type::Function; }

        [[nodiscard]] auto value() const { return m_fn; }

        auto apply(const size_t argc, mal::Data* const* args) const { return m_fn(argc, args); }

private:
        Fn m_fn{};
};

}  // namespace mal

#endif  // TYPES_HPP
