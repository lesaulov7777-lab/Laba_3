package ds

import (
	"bytes"
	"encoding/binary"
	"os"
	"sort"
	"strings"
	"testing"
)

func allContainers() map[string]func() Container {
	return map[string]func() Container{
		"MyArray":            func() Container { return NewMyArray(2) },
		"SinglyLinkedList":   func() Container { return &SinglyLinkedList{} },
		"DoublyLinkedList":   func() Container { return &DoublyLinkedList{} },
		"Queue":              func() Container { return &Queue{} },
		"Stack":              func() Container { return &Stack{} },
		"HashTable":          func() Container { return NewHashTable(8) },
		"CompleteBinaryTree": func() Container { return &CompleteBinaryTree{} },
	}
}

func sortedItems(c Container) []string {
	s := c.ToSlice()
	sort.Strings(s)
	return s
}

func fill(c Container, vals ...string) {
	for _, v := range vals {
		_ = c.Append(v)
	}
}

func TestRoundTripAllTypes(t *testing.T) {
	for name, mk := range allContainers() {
		t.Run(name, func(t *testing.T) {
			src := mk()
			fill(src, "5", "-3", "42", "7", "0")
			var bin, txt bytes.Buffer
			if err := SaveBinary(src, &bin); err != nil {
				t.Fatal(err)
			}
			if err := SaveText(src, &txt); err != nil {
				t.Fatal(err)
			}
			a, b := mk(), mk()
			fill(a, "999") // старое содержимое должно исчезнуть
			if err := LoadBinary(a, &bin); err != nil {
				t.Fatal(err)
			}
			if err := LoadText(b, &txt); err != nil {
				t.Fatal(err)
			}
			eq(t, sortedItems(a), sortedItems(src))
			eq(t, sortedItems(b), sortedItems(src))
		})
	}
}

func TestEmptyRoundTrip(t *testing.T) {
	for name, mk := range allContainers() {
		t.Run(name, func(t *testing.T) {
			var bin, txt bytes.Buffer
			_ = SaveBinary(mk(), &bin)
			_ = SaveText(mk(), &txt)
			a, b := mk(), mk()
			fill(a, "1")
			fill(b, "1")
			if err := LoadBinary(a, &bin); err != nil || a.Size() != 0 {
				t.Fatalf("binary: %v size=%d", err, a.Size())
			}
			if err := LoadText(b, &txt); err != nil || b.Size() != 0 {
				t.Fatalf("text: %v size=%d", err, b.Size())
			}
		})
	}
}

func TestTruncatedInputFailsAndKeepsData(t *testing.T) {
	src := &MyArray{}
	fill(src, "10", "20")
	var full bytes.Buffer
	_ = SaveBinary(src, &full)
	data := full.Bytes()
	for _, cut := range []int{0, 2, 4, 5, 8, 12, len(data) - 1} {
		dst := NewMyArray(2)
		fill(dst, "77")
		if err := LoadBinary(dst, bytes.NewReader(data[:cut])); err == nil {
			t.Fatalf("cut=%d: expected error", cut)
		}
		eq(t, dst.ToSlice(), V{"77"})
	}
	var txt bytes.Buffer
	_ = SaveText(src, &txt)
	s := txt.String()
	for _, in := range []string{"", s[:len(s)-4], "LR3T MyArray x\n", "LR3T MyArray -1\n", "BAD MyArray 0\n", "LR3T MyArray\n"} {
		if err := LoadText(NewMyArray(2), strings.NewReader(in)); err == nil {
			t.Fatalf("text %q: expected error", in)
		}
	}
	// заголовок без перевода строки в конце файла
	if err := LoadText(NewMyArray(2), strings.NewReader("LR3T MyArray 0")); err != nil {
		t.Fatalf("header without newline: %v", err)
	}
}

func TestTypeMismatchAndBadMagic(t *testing.T) {
	var s Stack
	s.Push("1")
	var bin, txt bytes.Buffer
	_ = SaveBinary(&s, &bin)
	_ = SaveText(&s, &txt)
	var q Queue
	if err := LoadBinary(&q, &bin); err == nil {
		t.Fatal("binary type mismatch accepted")
	}
	if err := LoadText(&q, &txt); err == nil {
		t.Fatal("text type mismatch accepted")
	}
	if err := LoadBinary(&q, strings.NewReader("XXXX1234")); err == nil {
		t.Fatal("bad magic accepted")
	}
}

func TestBadEscapeAndHugeLength(t *testing.T) {
	a := NewMyArray(2)
	for _, in := range []string{"LR3T MyArray 1\nabc\\\n", "LR3T MyArray 1\nab\\q\n"} {
		if err := LoadText(a, strings.NewReader(in)); err == nil {
			t.Fatalf("%q: expected error", in)
		}
	}
	var b bytes.Buffer
	b.WriteString("LR3B")
	b.WriteByte(7)
	b.WriteString("MyArray")
	_ = binary.Write(&b, binary.LittleEndian, uint32(1))
	_ = binary.Write(&b, binary.LittleEndian, uint32(0xFFFFFFFF))
	if err := LoadBinary(a, &b); err == nil {
		t.Fatal("huge element accepted")
	}
}

func TestSpecialCharactersSurvive(t *testing.T) {
	a := NewMyArray(2)
	fill(a, "hello world", "line1\nline2", `back\slash`, "cr\rhere", "", "юникод ✓")
	var bin, txt bytes.Buffer
	_ = SaveBinary(a, &bin)
	_ = SaveText(a, &txt)
	x, y := NewMyArray(2), NewMyArray(2)
	if err := LoadText(x, &txt); err != nil {
		t.Fatal(err)
	}
	if err := LoadBinary(y, &bin); err != nil {
		t.Fatal(err)
	}
	eq(t, x.ToSlice(), a.ToSlice())
	eq(t, y.ToSlice(), a.ToSlice())
}

func TestTreeRejectsNonNumericAndRollsBack(t *testing.T) {
	var tr CompleteBinaryTree
	tr.Insert(1)
	err := LoadText(&tr, strings.NewReader("LR3T CompleteBinaryTree 2\n5\nabc\n"))
	if err == nil {
		t.Fatal("non-numeric accepted")
	}
	eq(t, tr.LevelOrder(), []int{1}) // откат к прежнему состоянию
}

func TestOrderPreserved(t *testing.T) {
	var s Stack
	var q Queue
	fill(&s, "c", "a", "b")
	fill(&q, "c", "a", "b")
	var sb, qb bytes.Buffer
	_ = SaveText(&s, &sb)
	_ = SaveText(&q, &qb)
	var s2 Stack
	var q2 Queue
	_ = LoadText(&s2, &sb)
	_ = LoadText(&q2, &qb)
	top, _ := s2.Pop()
	front, _ := q2.Pop()
	eq(t, top+front, "bc")
}

type failWriter struct{}

func (failWriter) Write([]byte) (int, error) { return 0, os.ErrClosed }

func TestWriteErrorsPropagate(t *testing.T) {
	var q Queue
	if SaveBinary(&q, failWriter{}) == nil || SaveText(&q, failWriter{}) == nil {
		t.Fatal("write error swallowed")
	}
}

// Файлы, созданные C++-версией (make -C ../cpp run), должны читаться Go-версией.
func TestReadFilesWrittenByCpp(t *testing.T) {
	const dir = "../../cpp/out/"
	for name, mk := range allContainers() {
		bin, err1 := os.Open(dir + name + ".bin")
		txt, err2 := os.Open(dir + name + ".txt")
		if err1 != nil || err2 != nil {
			t.Skip("cpp/out отсутствует - выполните `make -C ../cpp run`")
		}
		a, b := mk(), mk()
		if err := LoadBinary(a, bin); err != nil {
			t.Fatalf("%s bin: %v", name, err)
		}
		if err := LoadText(b, txt); err != nil {
			t.Fatalf("%s txt: %v", name, err)
		}
		bin.Close()
		txt.Close()
		eq(t, sortedItems(a), V{"10", "20", "30", "40", "50"})
		eq(t, sortedItems(b), V{"10", "20", "30", "40", "50"})
	}
}
