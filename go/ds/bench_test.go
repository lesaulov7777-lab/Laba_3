package ds

import (
	"strconv"
	"testing"
)

func BenchmarkMyArrayPushBack(b *testing.B) {
	a := NewMyArray(2)
	for i := 0; i < b.N; i++ {
		a.PushBack("v")
	}
}

func BenchmarkSinglyListPushBack(b *testing.B) {
	var l SinglyLinkedList
	for i := 0; i < b.N; i++ {
		l.PushBack("v")
	}
}

func BenchmarkDoublyListPushBack(b *testing.B) {
	var l DoublyLinkedList
	for i := 0; i < b.N; i++ {
		l.PushBack("v")
	}
}

func BenchmarkStackPushPop(b *testing.B) {
	var s Stack
	for i := 0; i < b.N; i++ {
		s.Push("v")
		_, _ = s.Pop()
	}
}

func BenchmarkQueuePushPop(b *testing.B) {
	var q Queue
	for i := 0; i < b.N; i++ {
		q.Push("v")
		_, _ = q.Pop()
	}
}

func BenchmarkHashTableInsert(b *testing.B) {
	h := NewHashTable(8)
	for i := 0; i < b.N; i++ {
		h.Insert(strconv.Itoa(i))
	}
}

func BenchmarkHashTableContains(b *testing.B) {
	h := NewHashTable(8)
	for i := 0; i < 10000; i++ {
		h.Insert(strconv.Itoa(i))
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		h.Contains(strconv.Itoa(i % 10000))
	}
}

func BenchmarkTreeInsert(b *testing.B) {
	for _, n := range []int{500, 2000} {
		b.Run("n="+strconv.Itoa(n), func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				var t CompleteBinaryTree
				for k := 0; k < n; k++ {
					t.Insert(k)
				}
			}
		})
	}
}

func BenchmarkSerializeBinary(b *testing.B) {
	a := NewMyArray(2)
	for i := 0; i < 1000; i++ {
		a.PushBack(strconv.Itoa(i))
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		var buf nopWriter
		_ = SaveBinary(a, &buf)
	}
}

type nopWriter struct{ n int }

func (w *nopWriter) Write(p []byte) (int, error) { w.n += len(p); return len(p), nil }
