#ifndef LR3_QUEUE_H
#define LR3_QUEUE_H

#include <string>
#include <vector>

#include "IContainer.h"
#include "SinglyLinkedList.h"

namespace lr3 {

// Очередь FIFO на основе односвязного списка с хвостом: push и pop за O(1).
class Queue : public IContainer {
public:
    void push(const std::string& value) { list_.pushBack(value); }
    std::string pop() { return list_.popFront(); }  // std::out_of_range на пустой очереди
    const std::string& front() const { return list_.front(); }

    std::string typeName() const override { return "Queue"; }
    std::size_t size() const override { return list_.size(); }
    std::vector<std::string> toVector() const override { return list_.toVector(); }
    void clear() override { list_.clear(); }
    void append(const std::string& value) override { push(value); }

private:
    SinglyLinkedList list_;
};

}  // namespace lr3

#endif
