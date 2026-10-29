#ifndef LR3_COMPLETEBINARYTREE_H
#define LR3_COMPLETEBINARYTREE_H

#include <cstddef>
#include <string>
#include <vector>

#include "IContainer.h"

namespace lr3 {

// Полное (complete) двоичное дерево: уровни заполняются слева направо, вставка за O(n) обходом в ширину.
class CompleteBinaryTree : public IContainer {
public:
    CompleteBinaryTree() = default;
    CompleteBinaryTree(const CompleteBinaryTree& other);
    CompleteBinaryTree(CompleteBinaryTree&& other) noexcept;
    CompleteBinaryTree& operator=(const CompleteBinaryTree& other);
    CompleteBinaryTree& operator=(CompleteBinaryTree&& other) noexcept;
    ~CompleteBinaryTree() override;

    void insert(int value);
    bool find(int value) const;
    bool isComplete() const;
    int height() const;  // -1 для пустого дерева

    std::vector<int> levelOrder() const;
    std::vector<int> inOrder() const;
    std::vector<int> preOrder() const;
    std::vector<int> postOrder() const;

    std::string typeName() const override { return "CompleteBinaryTree"; }
    std::size_t size() const override { return size_; }
    std::vector<std::string> toVector() const override;
    void clear() override;
    void append(const std::string& value) override;  // std::invalid_argument, если это не целое число

private:
    struct Node {
        int value;
        Node* left;
        Node* right;
        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    static void destroy(Node* node);
    static int heightOf(const Node* node);
    static void inOrderRec(const Node* n, std::vector<int>& out);
    static void preOrderRec(const Node* n, std::vector<int>& out);
    static void postOrderRec(const Node* n, std::vector<int>& out);

    Node* root_ = nullptr;
    std::size_t size_ = 0;
};

}  // namespace lr3

#endif
