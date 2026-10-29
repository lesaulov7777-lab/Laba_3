// Демонстрация ЛР3: наполняем контейнеры, сохраняем в бинарном и текстовом форматах и читаем обратно.
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "CompleteBinaryTree.h"
#include "DoublyLinkedList.h"
#include "HashTable.h"
#include "MyArray.h"
#include "Queue.h"
#include "SinglyLinkedList.h"
#include "Stack.h"

using namespace lr3;

namespace {

void fill(IContainer& c) {
    for (const char* v : {"10", "20", "30", "40", "50"}) c.append(v);
}

int roundTrip(const std::string& dir, std::unique_ptr<IContainer> src, std::unique_ptr<IContainer> dstBin,
              std::unique_ptr<IContainer> dstText) {
    fill(*src);
    const std::string base = dir + "/" + src->typeName();
    {
        std::ofstream bin(base + ".bin", std::ios::binary);
        std::ofstream txt(base + ".txt");
        src->saveBinary(bin);
        src->saveText(txt);
    }
    {
        std::ifstream bin(base + ".bin", std::ios::binary);
        std::ifstream txt(base + ".txt");
        dstBin->loadBinary(bin);
        dstText->loadText(txt);
    }
    const bool ok = dstBin->toVector() == src->toVector() && dstText->toVector() == src->toVector();
    std::cout << src->typeName() << ": ";
    src->print(std::cout);
    std::cout << "  binary+text round-trip " << (ok ? "OK" : "FAILED") << "\n";
    return ok ? 0 : 1;
}

template <typename T>
int run(const std::string& dir) {
    return roundTrip(dir, std::make_unique<T>(), std::make_unique<T>(), std::make_unique<T>());
}

}  // namespace

int main(int argc, char** argv) {  // NOLINT(bugprone-exception-escape): все исключения перехватываются ниже
    const std::string dir = argc > 1 ? argv[1] : ".";
    try {
        int bad = 0;
        bad += run<MyArray>(dir);
        bad += run<SinglyLinkedList>(dir);
        bad += run<DoublyLinkedList>(dir);
        bad += run<Queue>(dir);
        bad += run<Stack>(dir);
        bad += run<HashTable>(dir);
        bad += run<CompleteBinaryTree>(dir);
        return bad == 0 ? 0 : 1;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 2;
    } catch (...) {
        std::cerr << "unknown error\n";
        return 3;
    }
}
