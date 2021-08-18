#ifndef DATA_HPP
#define DATA_HPP

#include <string>

namespace mal
{

class Symbol;
class List;
class Vector;
class HashMap;
class Integer;
class Keyword;
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
                Keyword,
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
        Keyword*  keyword();
        String*   string();
        Function* function();
};

}  // namespace mal

#endif  // DATA_HPP
