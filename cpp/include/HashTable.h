#ifndef LR3_HASHTABLE_H
#define LR3_HASHTABLE_H

#include <cstddef>
#include <string>
#include <vector>

#include "IContainer.h"
#include "SinglyLinkedList.h"

namespace lr3 {

// Хеш-функция DJB2 (h = h * 33 + c).
std::size_t djb2(const std::string& key);

// Множество строк: хеш-таблица с цепочками, динамическое расширение при заполнении > 0.75.
class HashTable : public IContainer {
public:
    explicit HashTable(std::size_t buckets = 8);

    bool insert(const std::string& key);  // false, если ключ уже есть
    bool erase(const std::string& key);
    bool contains(const std::string& key) const;

    std::size_t bucketCount() const { return buckets_.size(); }
    std::size_t maxChainLength() const;

    std::string typeName() const override { return "HashTable"; }
    std::size_t size() const override { return size_; }
    std::vector<std::string> toVector() const override;
    void clear() override;
    void append(const std::string& value) override { insert(value); }

private:
    void rehash(std::size_t newCount);
    std::vector<SinglyLinkedList> buckets_;
    std::size_t size_ = 0;
};

}  // namespace lr3

#endif
