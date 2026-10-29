// Простой benchmark: время операций при росте n (подтверждает оценки O(1)/O(n) из отчёта).
#include <chrono>
#include <iostream>
#include <string>

#include "CompleteBinaryTree.h"
#include "DoublyLinkedList.h"
#include "HashTable.h"
#include "MyArray.h"
#include "Queue.h"
#include "SinglyLinkedList.h"
#include "Stack.h"

using Clock = std::chrono::steady_clock;

template <typename F>
double ms(F&& f) {
    const auto t0 = Clock::now();
    f();
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

int main() {
    using namespace lr3;
    std::cout << "operation,n,ms\n";
    for (int n : {1000, 10000, 100000}) {
        const std::string v = "value";
        { MyArray c; std::cout << "MyArray.pushBack," << n << "," << ms([&] { for (int i = 0; i < n; ++i) c.pushBack(v); }) << "\n"; }
        { SinglyLinkedList c; std::cout << "SinglyLinkedList.pushBack," << n << "," << ms([&] { for (int i = 0; i < n; ++i) c.pushBack(v); }) << "\n"; }
        { DoublyLinkedList c; std::cout << "DoublyLinkedList.pushBack," << n << "," << ms([&] { for (int i = 0; i < n; ++i) c.pushBack(v); }) << "\n"; }
        { Stack c; std::cout << "Stack.push+pop," << n << "," << ms([&] { for (int i = 0; i < n; ++i) c.push(v); for (int i = 0; i < n; ++i) c.pop(); }) << "\n"; }
        { Queue c; std::cout << "Queue.push+pop," << n << "," << ms([&] { for (int i = 0; i < n; ++i) c.push(v); for (int i = 0; i < n; ++i) c.pop(); }) << "\n"; }
        { HashTable c; std::cout << "HashTable.insert," << n << "," << ms([&] { for (int i = 0; i < n; ++i) c.insert(std::to_string(i)); }) << "\n";
          std::cout << "HashTable.contains," << n << "," << ms([&] { for (int i = 0; i < n; ++i) c.contains(std::to_string(i)); }) << "\n"; }
    }
    for (int n : {500, 2000, 8000}) {  // вставка в дерево за O(n), поэтому размеры меньше
        CompleteBinaryTree t;
        std::cout << "CompleteBinaryTree.insert," << n << "," << ms([&] { for (int i = 0; i < n; ++i) t.insert(i); }) << "\n";
    }
    return 0;
}
