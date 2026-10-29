package ds

import (
	"errors"
	"reflect"
	"testing"
)

type V = []string

func eq(t *testing.T, got, want any) {
	t.Helper()
	if !reflect.DeepEqual(got, want) {
		t.Fatalf("got %v, want %v", got, want)
	}
}

func TestMyArray(t *testing.T) {
	a := NewMyArray(0)
	for _, s := range []string{"a", "c", "d"} {
		a.PushBack(s)
	}
	if err := a.InsertAt(1, "b"); err != nil {
		t.Fatal(err)
	}
	_ = a.InsertAt(4, "e")
	_ = a.InsertAt(0, "z")
	eq(t, a.ToSlice(), V{"z", "a", "b", "c", "d", "e"})
	if a.Capacity() < a.Size() {
		t.Fatal("capacity < size")
	}
	_ = a.SetAt(0, "y")
	_ = a.RemoveAt(1)
	v, _ := a.GetAt(0)
	eq(t, v, "y")
	eq(t, a.Size(), 5)
	for _, err := range []error{
		a.InsertAt(-1, "x"), a.InsertAt(99, "x"), a.SetAt(5, "x"), a.RemoveAt(-1),
		func() error { _, e := a.GetAt(5); return e }(),
	} {
		if !errors.Is(err, ErrIndex) {
			t.Fatalf("want ErrIndex, got %v", err)
		}
	}
	a.Clear()
	eq(t, a.Size(), 0)
	zero := &MyArray{}
	zero.PushBack("x") // нулевое значение тоже работоспособно
	eq(t, zero.ToSlice(), V{"x"})
}

func TestSinglyList(t *testing.T) {
	var l SinglyLinkedList
	if _, err := l.PopFront(); !errors.Is(err, ErrEmpty) {
		t.Fatal("PopFront on empty")
	}
	if _, err := l.PopBack(); !errors.Is(err, ErrEmpty) {
		t.Fatal("PopBack on empty")
	}
	if _, err := l.Front(); !errors.Is(err, ErrEmpty) {
		t.Fatal("Front on empty")
	}
	if _, err := l.Back(); !errors.Is(err, ErrEmpty) {
		t.Fatal("Back on empty")
	}
	if l.InsertBefore("x", "y") || l.InsertAfter("x", "y") || l.RemoveByValue("x") || l.RemoveBefore("x") {
		t.Fatal("operations on empty list must fail")
	}
	l.PushBack("a")
	l.PushBack("c")
	l.InsertAfter("c", "d")
	l.InsertAfter("a", "b")
	l.InsertBefore("a", "0")
	l.InsertBefore("c", "bb")
	if l.InsertBefore("none", "q") {
		t.Fatal("InsertBefore missing target")
	}
	eq(t, l.ToSlice(), V{"0", "a", "b", "bb", "c", "d"})
	if b, _ := l.Back(); b != "d" {
		t.Fatal("tail not updated")
	}
	l.RemoveByValue("0")
	l.RemoveByValue("d")
	if l.RemoveByValue("zz") {
		t.Fatal("removed missing")
	}
	eq(t, l.ToSlice(), V{"a", "b", "bb", "c"})
	l.RemoveAfter("c")
	if l.RemoveAfter("c") || l.RemoveAfter("none") {
		t.Fatal("RemoveAfter tail/missing")
	}
	l.RemoveAfter("a") // удаляет b
	eq(t, l.ToSlice(), V{"a", "bb", "c"})
	l.RemoveBefore("c") // из середины
	l.PushBack("e")
	l.RemoveBefore("c") // удаляет голову
	eq(t, l.ToSlice(), V{"c", "e"})
	if l.RemoveBefore("c") || l.RemoveBefore("none") {
		t.Fatal("RemoveBefore head/missing")
	}
	if v, _ := l.GetAt(1); v != "e" {
		t.Fatal("GetAt")
	}
	if _, err := l.GetAt(2); !errors.Is(err, ErrIndex) {
		t.Fatal("GetAt range")
	}
	if !l.Contains("e") || l.Contains("q") {
		t.Fatal("Contains")
	}
	l.PopBack()
	l.PopBack()
	eq(t, l.Size(), 0)
	l.PushFront("x")
	eq(t, l.TypeName(), "SinglyLinkedList")
}

func TestDoublyList(t *testing.T) {
	var l DoublyLinkedList
	if _, err := l.PopFront(); !errors.Is(err, ErrEmpty) {
		t.Fatal("PopFront on empty")
	}
	if _, err := l.PopBack(); !errors.Is(err, ErrEmpty) {
		t.Fatal("PopBack on empty")
	}
	if _, err := l.Front(); !errors.Is(err, ErrEmpty) {
		t.Fatal("Front on empty")
	}
	if _, err := l.Back(); !errors.Is(err, ErrEmpty) {
		t.Fatal("Back on empty")
	}
	if l.InsertAfter("x", "y") || l.InsertBefore("x", "y") {
		t.Fatal("insert into empty")
	}
	l.PushBack("b")
	l.InsertBefore("b", "a")
	l.InsertAfter("b", "d")
	l.InsertAfter("b", "c")
	l.InsertBefore("c", "bb")
	l.PushFront("0")
	eq(t, l.ToSlice(), V{"0", "a", "b", "bb", "c", "d"})
	eq(t, l.ToSliceReversed(), V{"d", "c", "bb", "b", "a", "0"})
	l.RemoveAfter("a")
	l.RemoveBefore("d")
	if l.RemoveAfter("d") || l.RemoveAfter("none") || l.RemoveBefore("0") || l.RemoveBefore("none") {
		t.Fatal("edge removals must fail")
	}
	eq(t, l.ToSlice(), V{"0", "a", "bb", "d"})
	if !l.RemoveByValue("a") || l.RemoveByValue("a") {
		t.Fatal("RemoveByValue")
	}
	if v, _ := l.GetAt(0); v != "0" {
		t.Fatal("GetAt")
	}
	if _, err := l.GetAt(-1); !errors.Is(err, ErrIndex) {
		t.Fatal("GetAt range")
	}
	if !l.Contains("d") || l.Contains("q") {
		t.Fatal("Contains")
	}
	f, _ := l.Front()
	b, _ := l.Back()
	eq(t, f+b, "0d")
	l.PopFront()
	l.PopBack()
	eq(t, l.Size(), 1)
}

func TestStackQueue(t *testing.T) {
	var s Stack
	if _, err := s.Pop(); !errors.Is(err, ErrEmpty) {
		t.Fatal("Pop empty")
	}
	if _, err := s.Top(); !errors.Is(err, ErrEmpty) {
		t.Fatal("Top empty")
	}
	for _, v := range []string{"1", "2", "3"} {
		s.Push(v)
	}
	eq(t, s.ToSlice(), V{"1", "2", "3"})
	top, _ := s.Top()
	eq(t, top, "3")
	v, _ := s.Pop()
	eq(t, v, "3")
	s.Clear()
	eq(t, s.Size(), 0)

	var q Queue
	if _, err := q.Pop(); !errors.Is(err, ErrEmpty) {
		t.Fatal("Pop empty")
	}
	if _, err := q.Front(); !errors.Is(err, ErrEmpty) {
		t.Fatal("Front empty")
	}
	for _, v := range []string{"1", "2", "3"} {
		q.Push(v)
	}
	f, _ := q.Front()
	eq(t, f, "1")
	q.Pop()
	eq(t, q.ToSlice(), V{"2", "3"})
	q.Clear()
	q.Push("again")
	f, _ = q.Front()
	eq(t, f, "again")
}

func TestHashTable(t *testing.T) {
	h := NewHashTable(0)
	if !h.Insert("a") || h.Insert("a") || !h.Contains("a") || h.Contains("b") {
		t.Fatal("insert/contains")
	}
	if !h.Erase("a") || h.Erase("a") {
		t.Fatal("erase")
	}
	h = NewHashTable(2)
	before := h.BucketCount()
	for i := 0; i < 500; i++ {
		h.Insert("key" + string(rune('A'+i%26)) + string(rune('a'+i/26)))
	}
	if h.BucketCount() <= before || h.MaxChainLength() > 20 {
		t.Fatalf("rehash: buckets=%d chain=%d", h.BucketCount(), h.MaxChainLength())
	}
	eq(t, len(h.ToSlice()), h.Size())
	h.Clear()
	eq(t, h.Size(), 0)
	eq(t, Djb2(""), uint64(5381))
	eq(t, Djb2("a"), uint64(5381*33+'a'))
}

func TestTree(t *testing.T) {
	var tr CompleteBinaryTree
	if !tr.IsComplete() || tr.Height() != -1 || tr.Find(1) || len(tr.LevelOrder()) != 0 {
		t.Fatal("empty tree")
	}
	for i := 1; i <= 7; i++ {
		tr.Insert(i)
		if !tr.IsComplete() {
			t.Fatal("not complete")
		}
	}
	eq(t, tr.LevelOrder(), []int{1, 2, 3, 4, 5, 6, 7})
	eq(t, tr.InOrder(), []int{4, 2, 5, 1, 6, 3, 7})
	eq(t, tr.PreOrder(), []int{1, 2, 4, 5, 3, 6, 7})
	eq(t, tr.PostOrder(), []int{4, 5, 2, 6, 7, 3, 1})
	eq(t, tr.Height(), 2)
	if !tr.Find(7) || tr.Find(8) {
		t.Fatal("find")
	}
	tr.Insert(8)
	eq(t, tr.Height(), 3)
	// искусственно ломаем структуру, чтобы проверить отрицательный результат IsComplete
	tr.root.left.right = nil
	if tr.IsComplete() {
		t.Fatal("broken tree reported complete")
	}
	if err := tr.Append("x"); err == nil {
		t.Fatal("Append must reject non-integers")
	}
	tr.Clear()
	eq(t, tr.Size(), 0)
}

func TestFormat(t *testing.T) {
	var q Queue
	q.Push("a")
	q.Push("b")
	eq(t, Format(&q), "[a, b]")
}
