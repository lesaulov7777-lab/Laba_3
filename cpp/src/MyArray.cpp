#include "MyArray.h"

#include <stdexcept>
#include <utility>

namespace lr3 {

MyArray::MyArray(std::size_t initialCapacity)
    : data_(nullptr), capacity_(initialCapacity == 0 ? 1 : initialCapacity), size_(0) {
    data_ = new std::string[capacity_];
}

MyArray::MyArray(const MyArray& other) : data_(new std::string[other.capacity_]), capacity_(other.capacity_), size_(other.size_) {
    for (std::size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
}

MyArray::MyArray(MyArray&& other) noexcept : data_(other.data_), capacity_(other.capacity_), size_(other.size_) {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
}

MyArray& MyArray::operator=(const MyArray& other) {
    if (this == &other) return *this;
    MyArray copy(other);
    *this = std::move(copy);
    return *this;
}

MyArray& MyArray::operator=(MyArray&& other) noexcept {
    if (this == &other) return *this;
    delete[] data_;
    data_ = other.data_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
    return *this;
}

MyArray::~MyArray() { delete[] data_; }

void MyArray::grow() {
    if (data_ != nullptr && size_ < capacity_) return;
    const std::size_t newCap = capacity_ == 0 ? 2 : capacity_ * 2;
    std::string* fresh = new std::string[newCap];
    for (std::size_t i = 0; i < size_; ++i) fresh[i] = std::move(data_[i]);  // NOLINT(clang-analyzer-core.NonNullParamChecker)
    delete[] data_;
    data_ = fresh;
    capacity_ = newCap;
}

void MyArray::checkIndex(std::size_t index) const {
    if (index >= size_) throw std::out_of_range("MyArray: index out of range");
}

void MyArray::pushBack(const std::string& value) {
    grow();
    data_[size_++] = value;
}

void MyArray::insertAt(std::size_t index, const std::string& value) {
    if (index > size_) throw std::out_of_range("MyArray: index out of range");
    grow();
    for (std::size_t i = size_; i > index; --i) data_[i] = std::move(data_[i - 1]);  // NOLINT(clang-analyzer-security.ArrayBound)
    data_[index] = value;
    ++size_;
}

const std::string& MyArray::getAt(std::size_t index) const {
    checkIndex(index);
    return data_[index];
}

void MyArray::setAt(std::size_t index, const std::string& value) {
    checkIndex(index);
    data_[index] = value;
}

void MyArray::removeAt(std::size_t index) {
    checkIndex(index);
    for (std::size_t i = index; i + 1 < size_; ++i) data_[i] = std::move(data_[i + 1]);  // NOLINT(clang-analyzer-security.ArrayBound)
    --size_;
}

std::vector<std::string> MyArray::toVector() const {
    std::vector<std::string> out;
    out.reserve(size_);
    for (std::size_t i = 0; i < size_; ++i) out.push_back(data_[i]);
    return out;
}

}  // namespace lr3
