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
                True,
                False,
                Nil,
                Function,
                FnList
        };

        enum class AllocType
        {
                Unique,
                Nude,
                Clone,
                Init
        };

        explicit Data(AllocType alloc) :
            m_alloc(alloc)
        {}

        [[nodiscard]] AllocType alloc_type() const { return m_alloc; }

        [[nodiscard]] virtual Type type() const = 0;

        [[nodiscard]] virtual bool is_truthy() const { return true; }

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

        [[nodiscard]] std::string format() const override { return std::to_string(m_int); }

        [[nodiscard]] Type type() const override { return Type::Integer; }

        [[nodiscard]] int value() const { return m_int; }

        int operator+(const Integer& rhs) const { return m_int + rhs.m_int; }

        int operator-(const Integer& rhs) const { return m_int - rhs.m_int; }

        int operator*(const Integer& rhs) const { return m_int * rhs.m_int; }

        int operator/(const Integer& rhs) const { return m_int / rhs.m_int; }

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

        explicit String(std::string string, AllocType alloc = AllocType::Unique) :
            Data(alloc), m_string{std::move(string)}
        {}

        String(String const& other) = default;
        String& operator=(String const& other) = default;

        String(String&& other) = default;
        String& operator=(String&& other) = default;

        ~String() override = default;

        [[nodiscard]] std::string format() const override { return m_string; }

        [[nodiscard]] Type type() const override { return Type::String; }

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

        [[nodiscard]] Type type() const override { return Type::List; }

        auto begin() { return m_list.begin(); }
        auto end() { return m_list.end(); }

        auto front() { return m_list.front().get(); }

        void push(DataPtr value) { m_list.push_back(std::move(value)); }

        [[nodiscard]] size_t size() const { return m_list.size(); }

        [[nodiscard]] bool empty() const { return m_list.empty(); }

        [[nodiscard]] mal::Data* at(std::size_t idx) const { return m_list.at(idx).get(); }

private:
        std::vector<DataPtr> m_list;
};

class CloneList : public Data
{
public:
        CloneList() = delete;

        explicit CloneList(AllocType alloc = AllocType::Clone) :
            Data(alloc)
        {}

        CloneList(CloneList const& other) = default;
        CloneList& operator=(CloneList const& other) = default;

        CloneList(CloneList&& other) = default;
        CloneList& operator=(CloneList&& other) = default;

        ~CloneList() override;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override { return Type::EvalList; }

        auto begin() { return m_clone_list.begin(); }
        auto end() { return m_clone_list.end(); }

        auto front() { return m_clone_list.front(); }

        void push(Data* value) { m_clone_list.push_back(value); }

        [[nodiscard]] size_t size() const { return m_clone_list.size(); }

        [[nodiscard]] mal::Data* at(std::size_t idx) const { return m_clone_list.at(idx); }

        [[nodiscard]] auto data() const { return m_clone_list.data(); }

private:
        std::vector<Data*> m_clone_list;
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

        ~EvalList() override;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override { return Type::EvalList; }

        auto begin() { return m_eval_list.begin(); }
        auto end() { return m_eval_list.end(); }

        auto front() { return m_eval_list.front(); }

        void push(Data* value) { m_eval_list.push_back(value); }

        [[nodiscard]] size_t size() const { return m_eval_list.size(); }

        [[nodiscard]] mal::Data* at(std::size_t idx) const { return m_eval_list.at(idx); }

        [[nodiscard]] auto data() const { return m_eval_list.data(); }

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

        [[nodiscard]] Type type() const override { return Type::Vector; }

        auto begin() { return m_vec.begin(); }
        auto end() { return m_vec.end(); }

        void push(DataPtr value) { m_vec.push_back(std::move(value)); }

        [[nodiscard]] size_t size() const { return m_vec.size(); }

        [[nodiscard]] bool empty() const { return m_vec.empty(); }

        [[nodiscard]] mal::Data* at(std::size_t idx) const { return m_vec.at(idx).get(); }

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

        ~EvalVector() override;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override { return Type::EvalVector; }

        auto begin() { return m_eval_vec.begin(); }
        auto end() { return m_eval_vec.end(); }

        void push(Data* value) { m_eval_vec.push_back(value); }

private:
        std::vector<Data*> m_eval_vec;
};

class CloneVector : public Data
{
public:
        CloneVector() = delete;

        explicit CloneVector(AllocType alloc = AllocType::Nude) :
            Data(alloc)
        {}

        CloneVector(CloneVector const& other) = default;
        CloneVector& operator=(CloneVector const& other) = default;

        CloneVector(CloneVector&& other) = default;
        CloneVector& operator=(CloneVector&& other) = default;

        ~CloneVector() override;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override { return Type::EvalVector; }

        auto begin() { return m_clone_vec.begin(); }
        auto end() { return m_clone_vec.end(); }

        void push(Data* value) { m_clone_vec.push_back(value); }

private:
        std::vector<Data*> m_clone_vec;
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

        [[nodiscard]] Type type() const override { return Type::HashMap; }

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

        ~EvalHashMap() override;

        [[nodiscard]] std::string format() const override;

        [[nodiscard]] Type type() const override { return Type::EvalHashMap; }

        auto begin() { return m_eval_map.begin(); }
        auto end() { return m_eval_map.end(); }

        void insert(mal::Data* key, mal::Data* value) { m_eval_map.emplace(key, value); }

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

class True : public Data
{
public:
        True() = delete;

        explicit True(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        True(True const& other) = default;
        True& operator=(True const& other) = default;

        True(True&& other) = default;
        True& operator=(True&& other) = default;

        ~True() override = default;

        [[nodiscard]] std::string format() const override { return "true"; }

        [[nodiscard]] Type type() const override { return Type::True; }
};

class False : public Data
{
public:
        False() = delete;

        explicit False(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        False(False const& other) = default;
        False& operator=(False const& other) = default;

        False(False&& other) = default;
        False& operator=(False&& other) = default;

        ~False() override = default;

        [[nodiscard]] std::string format() const override { return "false"; }

        [[nodiscard]] Type type() const override { return Type::False; }

        [[nodiscard]] bool is_truthy() const override { return false; }
};

class Nil : public Data
{
public:
        Nil() = delete;

        explicit Nil(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        Nil(Nil const& other) = default;
        Nil& operator=(Nil const& other) = default;

        Nil(Nil&& other) = default;
        Nil& operator=(Nil&& other) = default;

        ~Nil() override = default;

        [[nodiscard]] std::string format() const override
        {
                return "nil";
        }

        [[nodiscard]] Type type() const override { return Type::Nil; }

        [[nodiscard]] bool is_truthy() const override { return false; }
};

class Function : public Data
{
public:
        using Fn = std::function<mal::Data*(const std::size_t argc, mal::Data* const* args)>;

        Function() = delete;

        explicit Function(Fn fn, AllocType alloc = AllocType::Nude) :
            Data(alloc),
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

        auto apply(const size_t argc, mal::Data* const* args) const -> mal::Data*
        {
                return m_fn(argc, args);
        }

private:
        const Fn m_fn{};
};

class FnList : public Data
{
public:
        FnList() = delete;

        explicit FnList(AllocType alloc = AllocType::Unique) :
            Data(alloc)
        {}

        FnList(FnList const& other) = default;
        FnList& operator=(FnList const& other) = default;

        FnList(FnList&& other) = default;
        FnList& operator=(FnList&& other) = default;

        ~FnList() override = default;

        [[nodiscard]] std::string format() const override { return "formatting fnlist"; }

        [[nodiscard]] Type type() const override { return Type::FnList; }

        auto begin() { return m_fn_list.begin(); }
        auto end() { return m_fn_list.end(); }

        auto front() { return m_fn_list.front(); }

        void push(Data* value) { m_fn_list.push_back(value); }

        [[nodiscard]] size_t size() const { return m_fn_list.size(); }

        [[nodiscard]] mal::Data* at(std::size_t idx) const { return m_fn_list.at(idx); }

        [[nodiscard]] auto data() const { return m_fn_list.data(); }

private:
        std::vector<Data*> m_fn_list;
};

}  // namespace mal

#endif  // TYPES_HPP
