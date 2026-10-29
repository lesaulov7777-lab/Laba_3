#ifndef LR3_STACK_H
#define LR3_STACK_H

#include <string>
#include <vector>

#include "IContainer.h"
#include "SinglyLinkedList.h"

namespace lr3 {

// Стек на основе односвязного списка (вершина - голова списка). Композиция.
class Stack : public IContainer {
public:
    void push(const std::string& value) { list_.pushFront(value); }
    std::string pop() { return list_.popFront(); }  // std::out_of_range на пустом стеке
    const std::string& top() const { return list_.front(); }

    std::string typeName() const override { return "Stack"; }
    std::size_t size() const override { return list_.size(); }
    std::vector<std::string> toVector() const override;  // от дна к вершине
    void clear() override { list_.clear(); }
    void append(const std::string& value) override { push(value); }

private:
    SinglyLinkedList list_;
};

}  // namespace lr3

#endif
