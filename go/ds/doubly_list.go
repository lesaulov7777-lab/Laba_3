package ds

type doublyNode struct {
	value      string
	next, prev *doublyNode
}

// DoublyLinkedList - двусвязный список.
type DoublyLinkedList struct {
	head, tail *doublyNode
	size       int
}

func (l *DoublyLinkedList) TypeName() string { return "DoublyLinkedList" }
func (l *DoublyLinkedList) Size() int        { return l.size }
func (l *DoublyLinkedList) Clear()           { l.head, l.tail, l.size = nil, nil, 0 }

func (l *DoublyLinkedList) PushFront(v string) {
	n := &doublyNode{value: v, next: l.head}
	if l.head != nil {
		l.head.prev = n
	}
	l.head = n
	if l.tail == nil {
		l.tail = n
	}
	l.size++
}

func (l *DoublyLinkedList) PushBack(v string) {
	n := &doublyNode{value: v, prev: l.tail}
	if l.tail != nil {
		l.tail.next = n
	}
	l.tail = n
	if l.head == nil {
		l.head = n
	}
	l.size++
}

func (l *DoublyLinkedList) find(v string) *doublyNode {
	for n := l.head; n != nil; n = n.next {
		if n.value == v {
			return n
		}
	}
	return nil
}

func (l *DoublyLinkedList) unlink(n *doublyNode) {
	if n.prev != nil {
		n.prev.next = n.next
	} else {
		l.head = n.next
	}
	if n.next != nil {
		n.next.prev = n.prev
	} else {
		l.tail = n.prev
	}
	l.size--
}

func (l *DoublyLinkedList) Contains(v string) bool { return l.find(v) != nil }

func (l *DoublyLinkedList) InsertAfter(target, value string) bool {
	t := l.find(target)
	if t == nil {
		return false
	}
	n := &doublyNode{value: value, prev: t, next: t.next}
	if t.next != nil {
		t.next.prev = n
	} else {
		l.tail = n
	}
	t.next = n
	l.size++
	return true
}

func (l *DoublyLinkedList) InsertBefore(target, value string) bool {
	t := l.find(target)
	if t == nil {
		return false
	}
	n := &doublyNode{value: value, next: t, prev: t.prev}
	if t.prev != nil {
		t.prev.next = n
	} else {
		l.head = n
	}
	t.prev = n
	l.size++
	return true
}

func (l *DoublyLinkedList) PopFront() (string, error) {
	if l.head == nil {
		return "", ErrEmpty
	}
	v := l.head.value
	l.unlink(l.head)
	return v, nil
}

func (l *DoublyLinkedList) PopBack() (string, error) {
	if l.tail == nil {
		return "", ErrEmpty
	}
	v := l.tail.value
	l.unlink(l.tail)
	return v, nil
}

func (l *DoublyLinkedList) RemoveByValue(v string) bool {
	n := l.find(v)
	if n == nil {
		return false
	}
	l.unlink(n)
	return true
}

func (l *DoublyLinkedList) RemoveAfter(target string) bool {
	t := l.find(target)
	if t == nil || t.next == nil {
		return false
	}
	l.unlink(t.next)
	return true
}

func (l *DoublyLinkedList) RemoveBefore(target string) bool {
	t := l.find(target)
	if t == nil || t.prev == nil {
		return false
	}
	l.unlink(t.prev)
	return true
}

func (l *DoublyLinkedList) GetAt(index int) (string, error) {
	if index < 0 || index >= l.size {
		return "", ErrIndex
	}
	n := l.head
	for i := 0; i < index; i++ {
		n = n.next
	}
	return n.value, nil
}

func (l *DoublyLinkedList) Front() (string, error) {
	if l.head == nil {
		return "", ErrEmpty
	}
	return l.head.value, nil
}

func (l *DoublyLinkedList) Back() (string, error) {
	if l.tail == nil {
		return "", ErrEmpty
	}
	return l.tail.value, nil
}

func (l *DoublyLinkedList) ToSlice() []string {
	out := make([]string, 0, l.size)
	for n := l.head; n != nil; n = n.next {
		out = append(out, n.value)
	}
	return out
}

// ToSliceReversed обходит список от хвоста к голове.
func (l *DoublyLinkedList) ToSliceReversed() []string {
	out := make([]string, 0, l.size)
	for n := l.tail; n != nil; n = n.prev {
		out = append(out, n.value)
	}
	return out
}

func (l *DoublyLinkedList) Append(v string) error {
	l.PushBack(v)
	return nil
}
