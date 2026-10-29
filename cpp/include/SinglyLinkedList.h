#ifndef LR3_SINGLYLINKEDLIST_H
#define LR3_SINGLYLINKEDLIST_H

#include <cstddef>
#include <string>
#include <vector>

#include "IContainer.h"

namespace lr3 {

// Односвязный список с указателями на голову и хвост.
class SinglyLinkedList : public IContainer {
public:
    SinglyLinkedList() = default;
    SinglyLinkedList(const SinglyLinkedList& other);
    SinglyLinkedList(SinglyLinkedList&& other) noexcept;
    SinglyLinkedList& operator=(const SinglyLinkedList& other);
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept;
    ~SinglyLinkedList() override;

    void pushFront(const std::string& value);
    void pushBack(const std::string& value);
    bool insertAfter(const std::string& target, const std::string& value);
    bool insertBefore(const std::string& target, const std::string& value);

    std::string popFront();  // бросает std::out_of_range на пустом списке
    std::string popBack();
    bool removeByValue(const std::string& value);  // первое вхождение
    bool removeAfter(const std::string& target);
    bool removeBefore(const std::string& target);

    bool contains(const std::string& value) const;
    const std::string& getAt(std::size_t index) const;
    const std::string& front() const;
    const std::string& back() const;

    std::string typeName() const override { return "SinglyLinkedList"; }
    std::size_t size() const override { return size_; }
    std::vector<std::string> toVector() const override;
    void clear() override;
    void append(const std::string& value) override { pushBack(value); }

private:
    struct Node {
        std::string value;
        Node* next;
        explicit Node(const std::string& v) : value(v), next(nullptr) {}
    };

    Node* findNode(const std::string& value) const;
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
};

}  // namespace lr3

#endif
