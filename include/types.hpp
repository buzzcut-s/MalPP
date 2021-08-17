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
class Vector;
class EvalVector;
class HashMap;
class EvalHashMap;
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
                EvalList,
                EvalVector,
                EvalHashMap,
                Function
        };

        enum class AllocType
        {
                Unique,
                Nude
        };

        explicit Data(AllocType alloc) :
            m_alloc(alloc)
        {}

        [[nodiscard]] AllocType alloc_type() const
        {
                return m_alloc;
        }

        [[nodiscard]] virtual Type type() const = 0;

        Symbol*      symbol();
        List*        list();
        Vector*      vector();
        HashMap*     hashmap();
        Integer*     integer();
        EvalList*    eval_list();
        EvalVector*  eval_vector();
        EvalHashMap* eval_hashmap();
        Function*    function();

private:
        AllocType m_alloc{};
};

using DataPtr = std::unique_ptr<mal::Data>;

class Integer : public Data
{
public:
        Integer() = delete;

        explicit Integer(int int_value, AllocType alloc = AllocType::Unique) :
            Data(alloc), m_int{int_value}
        {}

        Integer(Integer const& other) = default;
        Integer& operator=(Integer const& other) = default;

        Integer(Integer&& other) = default;
        Integer& operator=(Integer&& other) = default;

        ~Integer() override = default;

        [[nodiscard]] std::string format() const override
        {
                return std::to_string(m_int);
        }

        [[nodiscard]] Type type() const override
        {
                return Type::Integer;
        }

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

private:
        int m_int;
};

class Symbol : public Data
{
public:
        Symbol() = delete;

        explicit Symbol(std::string symbol, AllocType alloc = AllocType::Unique) :
            Data(alloc), m_symbol{std::move(symbol)}
        {}

        Symbol(Symbol const& other) = default;
        Symbol& operator=(Symbol const& other) = default;

        Symbol(Symbol&& other) = default;
        Symbol& operator=(Symbol&& other) = default;

        ~Symbol() override = default;

        [[nodiscard]] std::string format() const override
        {
                return m_symbol;
        }

        [[nodiscard]] Type type() const override
        {
                return Type::Symbol;
        }

private:
        std::string m_symbol;
};

class String : public Data
{
public:
        String() = delete;

        explicit String(std::string string, AllocType alloc = AllocType::Unique) :
            Data(alloc), m_string{std::move(string)}
        {}

        String(String const& other) = default;
        String& operator=(String const& other) = default;

        String(String&& other) = default;
        String& operator=(String&& other) = default;

        ~String() override = default;

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
        List() = delete;

        explicit List(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        List(List const& other) = default;
        List& operator=(List const& other) = default;

        List(List&& other) = default;
        List& operator=(List&& other) = default;

        ~List() override = default;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override
        {
                return Type::List;
        }

        auto begin() { return m_list.begin(); }
        auto end() { return m_list.end(); }

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

private:
        std::vector<DataPtr> m_list;
};

class EvalList : public Data
{
public:
        EvalList() = delete;

        explicit EvalList(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        EvalList(EvalList const& other) = default;
        EvalList& operator=(EvalList const& other) = default;

        EvalList(EvalList&& other) = default;
        EvalList& operator=(EvalList&& other) = default;

        ~EvalList() override
        {
                for (auto* p : m_eval_list)
                        // TODO(piush) Evaluate this again
                        // Fixes Functiion::apply() memory leak
                        if (p && p->alloc_type() == mal::Data::AllocType::Nude)
                                delete p;
        }

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override
        {
                return Type::EvalList;
        }

        auto begin() { return m_eval_list.begin(); }
        auto end() { return m_eval_list.end(); }

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
        Vector() = delete;

        explicit Vector(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        Vector(Vector const& other) = default;
        Vector& operator=(Vector const& other) = default;

        Vector(Vector&& other) = default;
        Vector& operator=(Vector&& other) = default;

        ~Vector() override = default;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override
        {
                return Type::Vector;
        }

        auto begin() { return m_vec.begin(); }
        auto end() { return m_vec.end(); }

        void push(DataPtr value)
        {
                m_vec.push_back(std::move(value));
        }

private:
        std::vector<DataPtr> m_vec;
};

class EvalVector : public Data
{
public:
        EvalVector() = delete;

        explicit EvalVector(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        EvalVector(EvalVector const& other) = default;
        EvalVector& operator=(EvalVector const& other) = default;

        EvalVector(EvalVector&& other) = default;
        EvalVector& operator=(EvalVector&& other) = default;

        ~EvalVector() override
        {
                for (auto* p : m_eval_vec)
                        // TODO(piush) Evaluate this again
                        // Fixes Functiion::apply() memory leak
                        if (p && p->alloc_type() == mal::Data::AllocType::Nude)
                                delete p;
        }

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override
        {
                return Type::EvalVector;
        }

        auto begin() { return m_eval_vec.begin(); }
        auto end() { return m_eval_vec.end(); }

        void push(Data* value)
        {
                m_eval_vec.push_back(value);
        }

private:
        std::vector<Data*> m_eval_vec;
};

class HashMap : public Data
{
public:
        HashMap() = delete;

        explicit HashMap(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        HashMap(HashMap const& other) = default;
        HashMap& operator=(HashMap const& other) = default;

        HashMap(HashMap&& other) = default;
        HashMap& operator=(HashMap&& other) = default;

        ~HashMap() override = default;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override
        {
                return Type::HashMap;
        }

        auto begin() { return m_hashmap.begin(); }
        auto end() { return m_hashmap.end(); }

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

private:
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

        std::unordered_map<DataPtr, DataPtr, DataHasher, DataPred> m_hashmap;
};

class EvalHashMap : public Data
{
public:
        EvalHashMap() = delete;

        explicit EvalHashMap(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        EvalHashMap(EvalHashMap const& other) = default;
        EvalHashMap& operator=(EvalHashMap const& other) = default;

        EvalHashMap(EvalHashMap&& other) = default;
        EvalHashMap& operator=(EvalHashMap&& other) = default;

        ~EvalHashMap() override
        {
                for (auto& [key, val] : m_eval_map)
                        // TODO(piush) Evaluate this again
                        // Fixes Functiion::apply() memory leak
                        if (val && val->alloc_type() == mal::Data::AllocType::Nude)
                                delete val;
        }

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override
        {
                return Type::EvalHashMap;
        }

        auto begin() { return m_eval_map.begin(); }
        auto end() { return m_eval_map.end(); }

        void insert(mal::Data* key, mal::Data* value)
        {
                m_eval_map.emplace(key, value);
        }

private:
        struct EvalDataHasher
        {
                std::size_t operator()(const mal::Data* key) const noexcept
                {
                        return std::hash<std::string>{}(key->format());
                }
        };

        struct EvalDataPred
        {
                // TODO(piyush) Implement this, for real (equality)
                bool operator()(const mal::Data* lhs, const mal::Data* rhs) const
                {
                        // TODO(piyush) Changed this to check with string values. Ok?
                        return lhs->format() == rhs->format();
                }
        };

        std::unordered_map<mal::Data*, mal::Data*, EvalDataHasher, EvalDataPred> m_eval_map;
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

        [[nodiscard]] std::string format() const override { return "formatting function"; }

        [[nodiscard]] Type type() const override
        {
                return Type::Function;
        }

        [[nodiscard]] auto value() const
        {
                return m_fn;
        }

        auto apply(const size_t argc, mal::Data* const* args) const -> mal::Data*
        {
                return m_fn(argc, args);
        }

private:
        const Fn m_fn{};
};

}  // namespace mal

#endif  // TYPES_HPP
