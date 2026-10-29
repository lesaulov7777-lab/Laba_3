#include "IContainer.h"

#include <cstdint>
#include <istream>
#include <ostream>
#include <stdexcept>

namespace lr3 {

namespace {

const char kMagicBin[4] = {'L', 'R', '3', 'B'};
const char kMagicText[] = "LR3T";
const std::uint32_t kMaxLen = 64U * 1024U * 1024U;  // защита от повреждённых файлов

void writeU32(std::ostream& os, std::uint32_t v) {
    const char b[4] = {static_cast<char>(v & 0xFFU), static_cast<char>((v >> 8) & 0xFFU),
                       static_cast<char>((v >> 16) & 0xFFU), static_cast<char>((v >> 24) & 0xFFU)};
    os.write(b, 4);
}

std::uint32_t readU32(std::istream& is) {
    unsigned char b[4];
    is.read(reinterpret_cast<char*>(b), 4);
    if (!is) throw std::runtime_error("unexpected end of binary data");
    return static_cast<std::uint32_t>(b[0]) | (static_cast<std::uint32_t>(b[1]) << 8) |
           (static_cast<std::uint32_t>(b[2]) << 16) | (static_cast<std::uint32_t>(b[3]) << 24);
}

std::string escape(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else out += c;
    }
    return out;
}

std::string unescape(const std::string& s) {
    std::string out;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] != '\\') {
            out += s[i];
            continue;
        }
        if (++i >= s.size()) throw std::runtime_error("dangling escape in text data");
        if (s[i] == '\\') out += '\\';
        else if (s[i] == 'n') out += '\n';
        else if (s[i] == 'r') out += '\r';
        else throw std::runtime_error("unknown escape in text data");
    }
    return out;
}

}  // namespace

void IContainer::print(std::ostream& os) const {
    const std::vector<std::string> items = toVector();
    os << '[';
    for (std::size_t i = 0; i < items.size(); ++i) {
        if (i != 0) os << ", ";
        os << items[i];
    }
    os << "]\n";
}

void IContainer::saveBinary(std::ostream& os) const {
    const std::string name = typeName();
    const std::vector<std::string> items = toVector();
    os.write(kMagicBin, 4);
    os.put(static_cast<char>(name.size()));
    os.write(name.data(), static_cast<std::streamsize>(name.size()));
    writeU32(os, static_cast<std::uint32_t>(items.size()));
    for (const std::string& s : items) {
        writeU32(os, static_cast<std::uint32_t>(s.size()));
        os.write(s.data(), static_cast<std::streamsize>(s.size()));
    }
    if (!os) throw std::runtime_error("write error");
}

void IContainer::loadBinary(std::istream& is) {
    char magic[4];
    is.read(magic, 4);
    if (!is || std::string(magic, 4) != std::string(kMagicBin, 4)) throw std::runtime_error("bad binary magic");
    const int nameLen = is.get();
    if (nameLen == std::istream::traits_type::eof()) throw std::runtime_error("unexpected end of binary data");
    std::string name(static_cast<std::size_t>(nameLen), '\0');
    is.read(&name[0], nameLen);
    if (!is) throw std::runtime_error("unexpected end of binary data");
    if (name != typeName()) throw std::runtime_error("type mismatch: file has " + name + ", expected " + typeName());

    const std::uint32_t count = readU32(is);
    std::vector<std::string> items;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint32_t len = readU32(is);
        if (len > kMaxLen) throw std::runtime_error("element too large");
        std::string s(len, '\0');
        is.read(&s[0], static_cast<std::streamsize>(len));
        if (!is) throw std::runtime_error("unexpected end of binary data");
        items.push_back(std::move(s));
    }
    clear();  // контейнер меняется только после успешного чтения всего файла
    for (const std::string& s : items) append(s);
}

void IContainer::saveText(std::ostream& os) const {
    const std::vector<std::string> items = toVector();
    os << kMagicText << ' ' << typeName() << ' ' << items.size() << '\n';
    for (const std::string& s : items) os << escape(s) << '\n';
    if (!os) throw std::runtime_error("write error");
}

void IContainer::loadText(std::istream& is) {
    std::string magic;
    std::string name;
    std::size_t count = 0;
    if (!(is >> magic >> name >> count) || magic != kMagicText) throw std::runtime_error("bad text header");
    if (name != typeName()) throw std::runtime_error("type mismatch: file has " + name + ", expected " + typeName());
    std::string line;
    std::getline(is, line);  // остаток строки заголовка
    std::vector<std::string> items;
    for (std::size_t i = 0; i < count; ++i) {
        if (!std::getline(is, line)) throw std::runtime_error("unexpected end of text data");
        items.push_back(unescape(line));
    }
    clear();
    for (const std::string& s : items) append(s);
}

}  // namespace lr3
