package ds

import (
	"fmt"
	"strconv"
)

type treeNode struct {
	value       int
	left, right *treeNode
}

// CompleteBinaryTree - полное двоичное дерево: уровни заполняются слева направо.
type CompleteBinaryTree struct {
	root *treeNode
	size int
}

func (t *CompleteBinaryTree) TypeName() string { return "CompleteBinaryTree" }
func (t *CompleteBinaryTree) Size() int        { return t.size }
func (t *CompleteBinaryTree) Clear()           { t.root, t.size = nil, 0 }

// Insert добавляет значение в первую свободную позицию (обход в ширину).
func (t *CompleteBinaryTree) Insert(v int) {
	n := &treeNode{value: v}
	t.size++
	if t.root == nil {
		t.root = n
		return
	}
	queue := []*treeNode{t.root}
	for {
		cur := queue[0]
		queue = queue[1:]
		if cur.left == nil {
			cur.left = n
			return
		}
		if cur.right == nil {
			cur.right = n
			return
		}
		queue = append(queue, cur.left, cur.right)
	}
}

func (t *CompleteBinaryTree) Find(v int) bool {
	for _, x := range t.LevelOrder() {
		if x == v {
			return true
		}
	}
	return false
}

// IsComplete проверяет, что все уровни, кроме последнего, заполнены, а последний - слева направо.
func (t *CompleteBinaryTree) IsComplete() bool {
	queue := []*treeNode{t.root}
	seenNil := false
	for len(queue) > 0 {
		cur := queue[0]
		queue = queue[1:]
		if cur == nil {
			seenNil = true
			continue
		}
		if seenNil {
			return false
		}
		queue = append(queue, cur.left, cur.right)
	}
	return true
}

func height(n *treeNode) int {
	if n == nil {
		return -1
	}
	l, r := height(n.left), height(n.right)
	if r > l {
		l = r
	}
	return l + 1
}

// Height - высота дерева (-1 для пустого).
func (t *CompleteBinaryTree) Height() int { return height(t.root) }

func (t *CompleteBinaryTree) LevelOrder() []int {
	var out []int
	if t.root == nil {
		return out
	}
	queue := []*treeNode{t.root}
	for len(queue) > 0 {
		cur := queue[0]
		queue = queue[1:]
		out = append(out, cur.value)
		if cur.left != nil {
			queue = append(queue, cur.left)
		}
		if cur.right != nil {
			queue = append(queue, cur.right)
		}
	}
	return out
}

func walk(n *treeNode, order int, out *[]int) {
	if n == nil {
		return
	}
	if order == 0 { // pre
		*out = append(*out, n.value)
	}
	walk(n.left, order, out)
	if order == 1 { // in
		*out = append(*out, n.value)
	}
	walk(n.right, order, out)
	if order == 2 { // post
		*out = append(*out, n.value)
	}
}

func (t *CompleteBinaryTree) PreOrder() []int  { var o []int; walk(t.root, 0, &o); return o }
func (t *CompleteBinaryTree) InOrder() []int   { var o []int; walk(t.root, 1, &o); return o }
func (t *CompleteBinaryTree) PostOrder() []int { var o []int; walk(t.root, 2, &o); return o }

func (t *CompleteBinaryTree) ToSlice() []string {
	vals := t.LevelOrder()
	out := make([]string, len(vals))
	for i, v := range vals {
		out[i] = strconv.Itoa(v)
	}
	return out
}

func (t *CompleteBinaryTree) Append(s string) error {
	v, err := strconv.Atoi(s)
	if err != nil {
		return fmt.Errorf("ds: not an integer: %q", s)
	}
	t.Insert(v)
	return nil
}
