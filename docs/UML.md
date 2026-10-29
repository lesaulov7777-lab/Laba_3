# UML-диаграммы классов

## C++ (пространство имён `lr3`)

```mermaid
classDiagram
    class ISerializable {
        <<interface>>
        +saveBinary(ostream) void*
        +loadBinary(istream) void*
        +saveText(ostream) void*
        +loadText(istream) void*
    }
    class IContainer {
        <<abstract>>
        +typeName() string*
        +size() size_t*
        +toVector() vector~string~*
        +clear() void*
        +append(string) void*
        +empty() bool
        +print(ostream) void
        +saveBinary(ostream) void
        +loadBinary(istream) void
        +saveText(ostream) void
        +loadText(istream) void
    }
    class MyArray {
        -string* data_
        -size_t capacity_
        -size_t size_
        +pushBack(string)
        +insertAt(size_t, string)
        +getAt(size_t) string
        +setAt(size_t, string)
        +removeAt(size_t)
    }
    class SinglyLinkedList {
        -Node* head_
        -Node* tail_
        -size_t size_
        +pushFront(string)
        +pushBack(string)
        +insertAfter(string, string) bool
        +insertBefore(string, string) bool
        +popFront() string
        +popBack() string
        +removeByValue(string) bool
        +removeAfter(string) bool
        +removeBefore(string) bool
        +contains(string) bool
        +getAt(size_t) string
    }
    class DoublyLinkedList {
        -Node* head_
        -Node* tail_
        -size_t size_
        +pushFront(string)
        +pushBack(string)
        +popFront() string
        +popBack() string
        +toVectorReversed() vector~string~
    }
    class Stack {
        -SinglyLinkedList list_
        +push(string)
        +pop() string
        +top() string
    }
    class Queue {
        -SinglyLinkedList list_
        +push(string)
        +pop() string
        +front() string
    }
    class HashTable {
        -vector~SinglyLinkedList~ buckets_
        -size_t size_
        +insert(string) bool
        +erase(string) bool
        +contains(string) bool
        -rehash(size_t)
    }
    class CompleteBinaryTree {
        -Node* root_
        -size_t size_
        +insert(int)
        +find(int) bool
        +isComplete() bool
        +height() int
        +levelOrder() vector~int~
        +inOrder() vector~int~
        +preOrder() vector~int~
        +postOrder() vector~int~
    }
    ISerializable <|-- IContainer
    IContainer <|-- MyArray
    IContainer <|-- SinglyLinkedList
    IContainer <|-- DoublyLinkedList
    IContainer <|-- Stack
    IContainer <|-- Queue
    IContainer <|-- HashTable
    IContainer <|-- CompleteBinaryTree
    Stack *-- SinglyLinkedList : композиция
    Queue *-- SinglyLinkedList : композиция
    HashTable *-- "1..*" SinglyLinkedList : корзины
```

## Go (пакет `ds`)

В Go наследования нет: `Container` — интерфейс, который неявно реализуют все типы; сериализация — функции
пакета (`SaveBinary`, `LoadBinary`, `SaveText`, `LoadText`), принимающие `Container`.

```mermaid
classDiagram
    class Container {
        <<interface>>
        TypeName() string
        Size() int
        ToSlice() []string
        Clear()
        Append(string) error
    }
    Container <|.. MyArray
    Container <|.. SinglyLinkedList
    Container <|.. DoublyLinkedList
    Container <|.. Stack
    Container <|.. Queue
    Container <|.. HashTable
    Container <|.. CompleteBinaryTree
    Stack *-- SinglyLinkedList
    Queue *-- SinglyLinkedList
    HashTable *-- SinglyLinkedList
```
