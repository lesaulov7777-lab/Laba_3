#ifndef LR3_MYARRAY_H
#define LR3_MYARRAY_H

#include <cstddef>
#include <string>
#include <vector>

#include "IContainer.h"

namespace lr3 {

// Динамический массив строк с удвоением ёмкости.
class MyArray : public IContainer {
public:
    explicit MyArray(std::size_t initialCapacity = 2);
    MyArray(const MyArray& other);
    MyArray(MyArray&& other) noexcept;
    MyArray& operator=(const MyArray& other);
    MyArray& operator=(MyArray&& other) noexcept;
    ~MyArray() override;

    std::size_t capacity() const { return capacity_; }

    void pushBack(const std::string& value);
    void insertAt(std::size_t index, const std::string& value);  // index == size() допустим
    const std::string& getAt(std::size_t index) const;
    void setAt(std::size_t index, const std::string& value);
    void removeAt(std::size_t index);

    std::string typeName() const override { return "MyArray"; }
    std::size_t size() const override { return size_; }
    std::vector<std::string> toVector() const override;
    void clear() override { size_ = 0; }
    void append(const std::string& value) override { pushBack(value); }

private:
    void grow();
    void checkIndex(std::size_t index) const;

    std::string* data_;
    std::size_t capacity_;
    std::size_t size_;
};

}  // namespace lr3

#endif
