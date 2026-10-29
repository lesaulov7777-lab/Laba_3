#include "CompleteBinaryTree.h"

#include <algorithm>
#include <queue>
#include <stdexcept>
#include <utility>

namespace lr3 {

CompleteBinaryTree::CompleteBinaryTree(const CompleteBinaryTree& other) {
    for (int v : other.levelOrder()) insert(v);
}

CompleteBinaryTree::CompleteBinaryTree(CompleteBinaryTree&& other) noexcept : root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
    other.size_ = 0;
}

CompleteBinaryTree& CompleteBinaryTree::operator=(const CompleteBinaryTree& other) {
    if (this == &other) return *this;
    clear();
    for (int v : other.levelOrder()) insert(v);
    return *this;
}

CompleteBinaryTree& CompleteBinaryTree::operator=(CompleteBinaryTree&& other) noexcept {
    if (this == &other) return *this;
    clear();
    root_ = other.root_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.size_ = 0;
    return *this;
}

CompleteBinaryTree::~CompleteBinaryTree() { CompleteBinaryTree::clear(); }  // квалифицированный вызов: без виртуальной диспетчеризации в деструкторе

void CompleteBinaryTree::destroy(Node* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void CompleteBinaryTree::clear() {
    destroy(root_);
    root_ = nullptr;
    size_ = 0;
}

void CompleteBinaryTree::insert(int value) {
    if (root_ == nullptr) {
        root_ = new Node(value);
        size_ = 1;
        return;
    }
    // в обходе в ширину первый узел со свободной ячейкой - родитель нового узла
    std::queue<Node*> q;
    q.push(root_);
    for (;;) {
        Node* cur = q.front();
        q.pop();
        if (cur->left == nullptr) {
            cur->left = new Node(value);
            break;
        }
        if (cur->right == nullptr) {
            cur->right = new Node(value);
            break;
        }
        q.push(cur->left);
        q.push(cur->right);
    }
    ++size_;
}

bool CompleteBinaryTree::find(int value) const {
    if (root_ == nullptr) return false;
    std::queue<const Node*> q;
    q.push(root_);
    while (!q.empty()) {
        const Node* cur = q.front();
        q.pop();
        if (cur->value == value) return true;
        if (cur->left != nullptr) q.push(cur->left);
        if (cur->right != nullptr) q.push(cur->right);
    }
    return false;
}

bool CompleteBinaryTree::isComplete() const {
    std::queue<const Node*> q;
    q.push(root_);
    bool seenNull = false;
    while (!q.empty()) {
        const Node* cur = q.front();
        q.pop();
        if (cur == nullptr) {
            seenNull = true;
            continue;
        }
        if (seenNull) return false;
        q.push(cur->left);
        q.push(cur->right);
    }
    return true;
}

int CompleteBinaryTree::heightOf(const Node* node) {
    if (node == nullptr) return -1;
    return 1 + std::max(heightOf(node->left), heightOf(node->right));
}

int CompleteBinaryTree::height() const { return heightOf(root_); }

std::vector<int> CompleteBinaryTree::levelOrder() const {
    std::vector<int> out;
    if (root_ == nullptr) return out;
    std::queue<const Node*> q;
    q.push(root_);
    while (!q.empty()) {
        const Node* cur = q.front();
        q.pop();
        out.push_back(cur->value);
        if (cur->left != nullptr) q.push(cur->left);
        if (cur->right != nullptr) q.push(cur->right);
    }
    return out;
}

void CompleteBinaryTree::inOrderRec(const Node* n, std::vector<int>& out) {
    if (n == nullptr) return;
    inOrderRec(n->left, out);
    out.push_back(n->value);
    inOrderRec(n->right, out);
}

void CompleteBinaryTree::preOrderRec(const Node* n, std::vector<int>& out) {
    if (n == nullptr) return;
    out.push_back(n->value);
    preOrderRec(n->left, out);
    preOrderRec(n->right, out);
}

void CompleteBinaryTree::postOrderRec(const Node* n, std::vector<int>& out) {
    if (n == nullptr) return;
    postOrderRec(n->left, out);
    postOrderRec(n->right, out);
    out.push_back(n->value);
}

std::vector<int> CompleteBinaryTree::inOrder() const {
    std::vector<int> out;
    inOrderRec(root_, out);
    return out;
}

std::vector<int> CompleteBinaryTree::preOrder() const {
    std::vector<int> out;
    preOrderRec(root_, out);
    return out;
}

std::vector<int> CompleteBinaryTree::postOrder() const {
    std::vector<int> out;
    postOrderRec(root_, out);
    return out;
}

std::vector<std::string> CompleteBinaryTree::toVector() const {
    std::vector<std::string> out;
    for (int v : levelOrder()) out.push_back(std::to_string(v));
    return out;
}

void CompleteBinaryTree::append(const std::string& value) {
    std::size_t used = 0;
    const int v = std::stoi(value, &used);  // бросает invalid_argument / out_of_range
    if (used != value.size()) throw std::invalid_argument("not an integer: " + value);
    insert(v);
}

}  // namespace lr3
