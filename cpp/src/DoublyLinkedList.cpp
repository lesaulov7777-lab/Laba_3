#include "DoublyLinkedList.h"

#include <stdexcept>
#include <utility>

namespace lr3 {

DoublyLinkedList::DoublyLinkedList(const DoublyLinkedList& other) {
    for (Node* n = other.head_; n != nullptr; n = n->next) pushBack(n->value);
}

DoublyLinkedList::DoublyLinkedList(DoublyLinkedList&& other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

DoublyLinkedList& DoublyLinkedList::operator=(const DoublyLinkedList& other) {
    if (this == &other) return *this;
    clear();
    for (Node* n = other.head_; n != nullptr; n = n->next) pushBack(n->value);
    return *this;
}

DoublyLinkedList& DoublyLinkedList::operator=(DoublyLinkedList&& other) noexcept {
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

DoublyLinkedList::~DoublyLinkedList() { DoublyLinkedList::clear(); }  // квалифицированный вызов: без виртуальной диспетчеризации в деструкторе

void DoublyLinkedList::clear() {
    while (head_ != nullptr) {
        Node* next = head_->next;
        delete head_;
        head_ = next;
    }
    tail_ = nullptr;
    size_ = 0;
}

void DoublyLinkedList::pushFront(const std::string& value) {
    Node* node = new Node(value);
    node->next = head_;
    if (head_ != nullptr) head_->prev = node;
    head_ = node;
    if (tail_ == nullptr) tail_ = node;
    ++size_;
}

void DoublyLinkedList::pushBack(const std::string& value) {
    Node* node = new Node(value);
    node->prev = tail_;
    if (tail_ != nullptr) tail_->next = node;
    tail_ = node;
    if (head_ == nullptr) head_ = node;
    ++size_;
}

DoublyLinkedList::Node* DoublyLinkedList::findNode(const std::string& value) const {
    for (Node* n = head_; n != nullptr; n = n->next) {
        if (n->value == value) return n;
    }
    return nullptr;
}

void DoublyLinkedList::unlink(Node* node) {
    if (node->prev != nullptr) node->prev->next = node->next;
    else head_ = node->next;
    if (node->next != nullptr) node->next->prev = node->prev;
    else tail_ = node->prev;
    delete node;
    --size_;
}

bool DoublyLinkedList::insertAfter(const std::string& target, const std::string& value) {
    Node* t = findNode(target);
    if (t == nullptr) return false;
    Node* node = new Node(value);
    node->prev = t;
    node->next = t->next;
    if (t->next != nullptr) t->next->prev = node;
    else tail_ = node;
    t->next = node;
    ++size_;
    return true;
}

bool DoublyLinkedList::insertBefore(const std::string& target, const std::string& value) {
    Node* t = findNode(target);
    if (t == nullptr) return false;
    Node* node = new Node(value);
    node->next = t;
    node->prev = t->prev;
    if (t->prev != nullptr) t->prev->next = node;
    else head_ = node;
    t->prev = node;
    ++size_;
    return true;
}

std::string DoublyLinkedList::popFront() {
    if (head_ == nullptr) throw std::out_of_range("DoublyLinkedList: empty");
    std::string value = head_->value;
    unlink(head_);
    return value;
}

std::string DoublyLinkedList::popBack() {
    if (tail_ == nullptr) throw std::out_of_range("DoublyLinkedList: empty");
    std::string value = tail_->value;
    unlink(tail_);
    return value;
}

bool DoublyLinkedList::removeByValue(const std::string& value) {
    Node* n = findNode(value);
    if (n == nullptr) return false;
    unlink(n);
    return true;
}

bool DoublyLinkedList::removeAfter(const std::string& target) {
    Node* t = findNode(target);
    if (t == nullptr || t->next == nullptr) return false;
    unlink(t->next);
    return true;
}

bool DoublyLinkedList::removeBefore(const std::string& target) {
    Node* t = findNode(target);
    if (t == nullptr || t->prev == nullptr) return false;
    unlink(t->prev);
    return true;
}

bool DoublyLinkedList::contains(const std::string& value) const { return findNode(value) != nullptr; }

const std::string& DoublyLinkedList::getAt(std::size_t index) const {
    if (index >= size_) throw std::out_of_range("DoublyLinkedList: index out of range");
    Node* n = head_;
    for (std::size_t i = 0; i < index; ++i) n = n->next;
    return n->value;
}

const std::string& DoublyLinkedList::front() const {
    if (head_ == nullptr) throw std::out_of_range("DoublyLinkedList: empty");
    return head_->value;
}

const std::string& DoublyLinkedList::back() const {
    if (tail_ == nullptr) throw std::out_of_range("DoublyLinkedList: empty");
    return tail_->value;
}

std::vector<std::string> DoublyLinkedList::toVector() const {
    std::vector<std::string> out;
    out.reserve(size_);
    for (Node* n = head_; n != nullptr; n = n->next) out.push_back(n->value);
    return out;
}

std::vector<std::string> DoublyLinkedList::toVectorReversed() const {
    std::vector<std::string> out;
    out.reserve(size_);
    for (Node* n = tail_; n != nullptr; n = n->prev) out.push_back(n->value);
    return out;
}

}  // namespace lr3
