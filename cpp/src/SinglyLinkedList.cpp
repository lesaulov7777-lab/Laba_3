#include "SinglyLinkedList.h"

#include <stdexcept>
#include <utility>

namespace lr3 {

SinglyLinkedList::SinglyLinkedList(const SinglyLinkedList& other) {
    for (Node* n = other.head_; n != nullptr; n = n->next) pushBack(n->value);
}

SinglyLinkedList::SinglyLinkedList(SinglyLinkedList&& other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

SinglyLinkedList& SinglyLinkedList::operator=(const SinglyLinkedList& other) {
    if (this == &other) return *this;
    clear();
    for (Node* n = other.head_; n != nullptr; n = n->next) pushBack(n->value);
    return *this;
}

SinglyLinkedList& SinglyLinkedList::operator=(SinglyLinkedList&& other) noexcept {
    if (this == &other) return *this;
    clear();
    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
    return *this;
}

SinglyLinkedList::~SinglyLinkedList() { SinglyLinkedList::clear(); }  // квалифицированный вызов: без виртуальной диспетчеризации в деструкторе

void SinglyLinkedList::clear() {
    while (head_ != nullptr) {
        Node* next = head_->next;
        delete head_;
        head_ = next;
    }
    tail_ = nullptr;
    size_ = 0;
}

void SinglyLinkedList::pushFront(const std::string& value) {
    Node* node = new Node(value);
    node->next = head_;
    head_ = node;
    if (tail_ == nullptr) tail_ = node;
    ++size_;
}

void SinglyLinkedList::pushBack(const std::string& value) {
    Node* node = new Node(value);
    if (tail_ == nullptr) {
        head_ = tail_ = node;
    } else {
        tail_->next = node;
        tail_ = node;
    }
    ++size_;
}

SinglyLinkedList::Node* SinglyLinkedList::findNode(const std::string& value) const {
    for (Node* n = head_; n != nullptr; n = n->next) {
        if (n->value == value) return n;
    }
    return nullptr;
}

bool SinglyLinkedList::insertAfter(const std::string& target, const std::string& value) {
    Node* t = findNode(target);
    if (t == nullptr) return false;
    Node* node = new Node(value);
    node->next = t->next;
    t->next = node;
    if (tail_ == t) tail_ = node;
    ++size_;
    return true;
}

bool SinglyLinkedList::insertBefore(const std::string& target, const std::string& value) {
    if (head_ == nullptr) return false;
    if (head_->value == target) {
        pushFront(value);
        return true;
    }
    for (Node* n = head_; n->next != nullptr; n = n->next) {
        if (n->next->value == target) {
            Node* node = new Node(value);
            node->next = n->next;
            n->next = node;
            ++size_;
            return true;
        }
    }
    return false;
}

std::string SinglyLinkedList::popFront() {
    if (head_ == nullptr) throw std::out_of_range("SinglyLinkedList: empty");
    Node* node = head_;
    std::string value = node->value;
    head_ = node->next;
    if (head_ == nullptr) tail_ = nullptr;
    delete node;
    --size_;
    return value;
}

std::string SinglyLinkedList::popBack() {
    if (head_ == nullptr) throw std::out_of_range("SinglyLinkedList: empty");
    if (head_ == tail_) return popFront();
    Node* prev = head_;
    while (prev->next != tail_) prev = prev->next;
    std::string value = tail_->value;
    delete tail_;
    tail_ = prev;
    prev->next = nullptr;
    --size_;
    return value;
}

bool SinglyLinkedList::removeByValue(const std::string& value) {
    if (head_ == nullptr) return false;
    if (head_->value == value) {
        popFront();
        return true;
    }
    for (Node* n = head_; n->next != nullptr; n = n->next) {
        if (n->next->value == value) {
            Node* victim = n->next;
            n->next = victim->next;
            if (victim == tail_) tail_ = n;
            delete victim;
            --size_;
            return true;
        }
    }
    return false;
}

bool SinglyLinkedList::removeAfter(const std::string& target) {
    Node* t = findNode(target);
    if (t == nullptr || t->next == nullptr) return false;
    Node* victim = t->next;
    t->next = victim->next;
    if (victim == tail_) tail_ = t;
    delete victim;
    --size_;
    return true;
}

bool SinglyLinkedList::removeBefore(const std::string& target) {
    if (head_ == nullptr || head_->value == target) return false;
    if (head_->next != nullptr && head_->next->value == target) {
        popFront();
        return true;
    }
    for (Node* n = head_; n->next != nullptr && n->next->next != nullptr; n = n->next) {
        if (n->next->next->value == target) {
            Node* victim = n->next;
            n->next = victim->next;
            delete victim;
            --size_;
            return true;
        }
    }
    return false;
}

bool SinglyLinkedList::contains(const std::string& value) const { return findNode(value) != nullptr; }

const std::string& SinglyLinkedList::getAt(std::size_t index) const {
    if (index >= size_) throw std::out_of_range("SinglyLinkedList: index out of range");
    Node* n = head_;
    for (std::size_t i = 0; i < index; ++i) n = n->next;
    return n->value;
}

const std::string& SinglyLinkedList::front() const {
    if (head_ == nullptr) throw std::out_of_range("SinglyLinkedList: empty");
    return head_->value;
}

const std::string& SinglyLinkedList::back() const {
    if (tail_ == nullptr) throw std::out_of_range("SinglyLinkedList: empty");
    return tail_->value;
}

std::vector<std::string> SinglyLinkedList::toVector() const {
    std::vector<std::string> out;
    out.reserve(size_);
    for (Node* n = head_; n != nullptr; n = n->next) out.push_back(n->value);
    return out;
}

}  // namespace lr3
