#include "HashTable.h"

#include <algorithm>
#include <utility>

namespace lr3 {

std::size_t djb2(const std::string& key) {
    std::size_t h = 5381;
    for (unsigned char c : key) h = h * 33 + c;
    return h;
}

HashTable::HashTable(std::size_t buckets) : buckets_(std::max<std::size_t>(buckets, 1)) {}

bool HashTable::contains(const std::string& key) const { return buckets_[djb2(key) % buckets_.size()].contains(key); }

bool HashTable::insert(const std::string& key) {
    if (contains(key)) return false;
    if ((size_ + 1) * 4 > buckets_.size() * 3) rehash(buckets_.size() * 2);
    buckets_[djb2(key) % buckets_.size()].pushFront(key);
    ++size_;
    return true;
}

bool HashTable::erase(const std::string& key) {
    if (!buckets_[djb2(key) % buckets_.size()].removeByValue(key)) return false;
    --size_;
    return true;
}

void HashTable::rehash(std::size_t newCount) {
    std::vector<SinglyLinkedList> fresh(newCount);
    for (const SinglyLinkedList& b : buckets_) {
        for (const std::string& k : b.toVector()) fresh[djb2(k) % newCount].pushFront(k);
    }
    buckets_ = std::move(fresh);
}

std::size_t HashTable::maxChainLength() const {
    std::size_t mx = 0;
    for (const SinglyLinkedList& b : buckets_) mx = std::max(mx, b.size());
    return mx;
}

std::vector<std::string> HashTable::toVector() const {
    std::vector<std::string> out;
    out.reserve(size_);
    for (const SinglyLinkedList& b : buckets_) {
        for (const std::string& k : b.toVector()) out.push_back(k);
    }
    return out;
}

void HashTable::clear() {
    for (SinglyLinkedList& b : buckets_) b.clear();
    size_ = 0;
}

}  // namespace lr3
