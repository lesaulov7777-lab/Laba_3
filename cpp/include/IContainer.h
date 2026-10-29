#ifndef LR3_ICONTAINER_H
#define LR3_ICONTAINER_H

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

namespace lr3 {

// Абстрактный интерфейс сериализации (бинарный и текстовый форматы).
class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual void saveBinary(std::ostream& os) const = 0;
    virtual void loadBinary(std::istream& is) = 0;
    virtual void saveText(std::ostream& os) const = 0;
    virtual void loadText(std::istream& is) = 0;
};

// Общий абстрактный базовый класс всех контейнеров.
// Наследники реализуют только 5 примитивов, а сериализация делается здесь (шаблонный метод).
//
// Форматы (одинаковы для C++ и Go):
//   бинарный: "LR3B" | u8 len(typeName) | typeName | u32 count | count * (u32 len | bytes)  (little-endian)
//   текстовый: "LR3T <typeName> <count>\n" и далее по одной строке на элемент (\\ \n \r экранируются)
class IContainer : public ISerializable {
public:
    virtual std::string typeName() const = 0;
    virtual std::size_t size() const = 0;
    virtual std::vector<std::string> toVector() const = 0;  // порядок, достаточный для восстановления
    virtual void clear() = 0;
    virtual void append(const std::string& value) = 0;      // добавление при десериализации

    bool empty() const { return size() == 0; }
    void print(std::ostream& os) const;

    void saveBinary(std::ostream& os) const override;
    void loadBinary(std::istream& is) override;
    void saveText(std::ostream& os) const override;
    void loadText(std::istream& is) override;
};

}  // namespace lr3

#endif
