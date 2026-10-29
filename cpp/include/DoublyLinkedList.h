#ifndef LR3_DOUBLYLINKEDLIST_H
#define LR3_DOUBLYLINKEDLIST_H

#include <cstddef>
#include <string>
#include <vector>

#include "IContainer.h"

namespace lr3 {

// Двусвязный список.
class DoublyLinkedList : public IContainer {
public:
    DoublyLinkedList() = default;
    DoublyLinkedList(const DoublyLinkedList& other);
    DoublyLinkedList(DoublyLinkedList&& other) noexcept;
    DoublyLinkedList& operator=(const DoublyLinkedList& other);
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept;
    ~DoublyLinkedList() override;

    void pushFront(const std::string& value);
    void pushBack(const std::string& value);
    bool insertAfter(const std::string& target, const std::string& value);
    bool insertBefore(const std::string& target, const std::string& value);

    std::string popFront();  // бросает std::out_of_range на пустом списке
    std::string popBack();
    bool removeByValue(const std::string& value);
    bool removeAfter(const std::string& target);
    bool removeBefore(const std::string& target);

    bool contains(const std::string& value) const;
    const std::string& getAt(std::size_t index) const;
    const std::string& front() const;
    const std::string& back() const;
    std::vector<std::string> toVectorReversed() const;

    std::string typeName() const override { return "DoublyLinkedList"; }
    std::size_t size() const override { return size_; }
    std::vector<std::string> toVector() const override;
    void clear() override;
    void append(const std::string& value) override { pushBack(value); }

private:
    struct Node {
        std::string value;
        Node* next;
        Node* prev;
        explicit Node(const std::string& v) : value(v), next(nullptr), prev(nullptr) {}
    };

    Node* findNode(const std::string& value) const;
    void unlink(Node* node);
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
};

}  // namespace lr3

#endif
