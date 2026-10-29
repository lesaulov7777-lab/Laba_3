#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "CompleteBinaryTree.h"
#include "DoublyLinkedList.h"
#include "HashTable.h"
#include "MyArray.h"
#include "Queue.h"
#include "SinglyLinkedList.h"
#include "Stack.h"

using namespace lr3;
using V = std::vector<std::string>;

namespace {

V sorted(const IContainer& c) {
    V v = c.toVector();
    std::sort(v.begin(), v.end());
    return v;
}

template <typename T>
class SerializationTest : public ::testing::Test {};

using AllTypes = ::testing::Types<MyArray, SinglyLinkedList, DoublyLinkedList, Queue, Stack, HashTable, CompleteBinaryTree>;
TYPED_TEST_SUITE(SerializationTest, AllTypes);

}  // namespace

TYPED_TEST(SerializationTest, BinaryRoundTrip) {
    TypeParam src;
    for (const char* v : {"5", "-3", "42", "7", "0"}) src.append(v);
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    src.saveBinary(ss);
    TypeParam dst;
    dst.append("999");  // старое содержимое должно быть заменено
    dst.loadBinary(ss);
    EXPECT_EQ(sorted(dst), sorted(src));
    EXPECT_EQ(dst.size(), src.size());
}

TYPED_TEST(SerializationTest, TextRoundTrip) {
    TypeParam src;
    for (const char* v : {"1", "2", "3", "4"}) src.append(v);
    std::stringstream ss;
    src.saveText(ss);
    TypeParam dst;
    dst.loadText(ss);
    EXPECT_EQ(sorted(dst), sorted(src));
}

TYPED_TEST(SerializationTest, EmptyRoundTrip) {
    TypeParam src;
    std::stringstream bin(std::ios::in | std::ios::out | std::ios::binary);
    std::stringstream txt;
    src.saveBinary(bin);
    src.saveText(txt);
    TypeParam a;
    TypeParam b;
    a.append("1");
    b.append("1");
    a.loadBinary(bin);
    b.loadText(txt);
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.empty());
}

TYPED_TEST(SerializationTest, TruncatedBinaryFailsAndKeepsData) {
    TypeParam src;
    src.append("10");
    src.append("20");
    std::stringstream full(std::ios::in | std::ios::out | std::ios::binary);
    src.saveBinary(full);
    const std::string bytes = full.str();
    for (std::size_t cut : {std::size_t{0}, std::size_t{2}, std::size_t{5}, bytes.size() - 1}) {
        std::stringstream bad(bytes.substr(0, cut), std::ios::in | std::ios::binary);
        TypeParam dst;
        dst.append("77");
        EXPECT_THROW(dst.loadBinary(bad), std::runtime_error) << "cut=" << cut;
        EXPECT_EQ(dst.size(), 1U);  // содержимое не пострадало
    }
}

TYPED_TEST(SerializationTest, TruncatedTextFails) {
    TypeParam src;
    src.append("10");
    src.append("20");
    std::stringstream txt;
    src.saveText(txt);
    std::string s = txt.str();
    s.resize(s.size() - 3);  // обрезаем последнюю строку и перевод строки
    std::stringstream bad(s.substr(0, s.rfind('\n')));
    TypeParam dst;
    EXPECT_THROW(dst.loadText(bad), std::runtime_error);
}

TEST(Serialization, TypeMismatchRejected) {
    Stack s;
    s.append("1");
    std::stringstream bin(std::ios::in | std::ios::out | std::ios::binary);
    std::stringstream txt;
    s.saveBinary(bin);
    s.saveText(txt);
    Queue q;
    EXPECT_THROW(q.loadBinary(bin), std::runtime_error);
    EXPECT_THROW(q.loadText(txt), std::runtime_error);
}

TEST(Serialization, BadMagicRejected) {
    Queue q;
    std::stringstream bin("XXXX1234", std::ios::in | std::ios::binary);
    EXPECT_THROW(q.loadBinary(bin), std::runtime_error);
    std::stringstream txt("HELLO Queue 0\n");
    EXPECT_THROW(q.loadText(txt), std::runtime_error);
    std::stringstream empty;
    EXPECT_THROW(q.loadText(empty), std::runtime_error);
}

TEST(Serialization, BadEscapeAndHugeLengthRejected) {
    MyArray a;
    std::stringstream t1("LR3T MyArray 1\nabc\\");
    EXPECT_THROW(a.loadText(t1), std::runtime_error);
    std::stringstream t2("LR3T MyArray 1\nab\\q\n");
    EXPECT_THROW(a.loadText(t2), std::runtime_error);

    std::string huge("LR3B", 4);
    huge += static_cast<char>(7);
    huge += "MyArray";
    huge += std::string("\x01\x00\x00\x00", 4);  // один элемент
    huge += std::string("\xff\xff\xff\xff", 4);  // длина 4 ГБ
    std::stringstream h(huge, std::ios::in | std::ios::binary);
    EXPECT_THROW(a.loadBinary(h), std::runtime_error);
}

TEST(Serialization, SpecialCharactersSurvive) {
    MyArray a;
    a.append("hello world");
    a.append("line1\nline2");
    a.append("back\\slash");
    a.append("cr\rhere");
    a.append("");
    a.append("юникод ✓");
    std::stringstream txt;
    std::stringstream bin(std::ios::in | std::ios::out | std::ios::binary);
    a.saveText(txt);
    a.saveBinary(bin);
    MyArray t;
    MyArray b;
    t.loadText(txt);
    b.loadBinary(bin);
    EXPECT_EQ(t.toVector(), a.toVector());
    EXPECT_EQ(b.toVector(), a.toVector());
}

TEST(Serialization, OrderPreservedForOrderedContainers) {
    Stack s;
    Queue q;
    DoublyLinkedList d;
    for (const char* v : {"c", "a", "b"}) {
        s.push(v);
        q.push(v);
        d.pushBack(v);
    }
    std::stringstream ss;
    std::stringstream qs;
    std::stringstream ds;
    s.saveText(ss);
    q.saveText(qs);
    d.saveText(ds);
    Stack s2;
    Queue q2;
    DoublyLinkedList d2;
    s2.loadText(ss);
    q2.loadText(qs);
    d2.loadText(ds);
    EXPECT_EQ(s2.pop(), "b");
    EXPECT_EQ(q2.pop(), "c");
    EXPECT_EQ(d2.toVector(), (V{"c", "a", "b"}));
}

TEST(Serialization, TreeStructureRestored) {
    CompleteBinaryTree t;
    for (int i = 1; i <= 10; ++i) t.insert(i * 3);
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    t.saveBinary(ss);
    CompleteBinaryTree r;
    r.loadBinary(ss);
    EXPECT_EQ(r.levelOrder(), t.levelOrder());
    EXPECT_EQ(r.inOrder(), t.inOrder());
    EXPECT_TRUE(r.isComplete());
}

TEST(Serialization, TreeRejectsNonNumericText) {
    CompleteBinaryTree t;
    std::stringstream ss("LR3T CompleteBinaryTree 1\nabc\n");
    EXPECT_THROW(t.loadText(ss), std::invalid_argument);
}
