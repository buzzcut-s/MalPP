#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <string>
#include <unordered_map>

#include "../Data.hpp"

namespace mal
{

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
                size_t operator()(const mal::Data* key) const noexcept;
        };

        struct DataPred
        {
                bool operator()(const mal::Data* lhs, const mal::Data* rhs) const;
        };

        std::unordered_map<mal::Data*, mal::Data*, DataHasher, DataPred> m_hashmap;
};

}  // namespace mal

#endif  // HASHMAP_HPP
