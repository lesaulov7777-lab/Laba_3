package ds

type singlyNode struct {
	value string
	next  *singlyNode
}

// SinglyLinkedList - односвязный список с указателями на голову и хвост.
type SinglyLinkedList struct {
	head, tail *singlyNode
	size       int
}

func (l *SinglyLinkedList) TypeName() string { return "SinglyLinkedList" }
func (l *SinglyLinkedList) Size() int        { return l.size }
func (l *SinglyLinkedList) Clear()           { l.head, l.tail, l.size = nil, nil, 0 }

func (l *SinglyLinkedList) PushFront(v string) {
	n := &singlyNode{value: v, next: l.head}
	l.head = n
	if l.tail == nil {
		l.tail = n
	}
	l.size++
}

func (l *SinglyLinkedList) PushBack(v string) {
	n := &singlyNode{value: v}
	if l.tail == nil {
		l.head, l.tail = n, n
	} else {
		l.tail.next = n
		l.tail = n
	}
	l.size++
}

func (l *SinglyLinkedList) find(v string) *singlyNode {
	for n := l.head; n != nil; n = n.next {
		if n.value == v {
			return n
		}
	}
	return nil
}

func (l *SinglyLinkedList) Contains(v string) bool { return l.find(v) != nil }

// InsertAfter вставляет value после первого вхождения target.
func (l *SinglyLinkedList) InsertAfter(target, value string) bool {
	t := l.find(target)
	if t == nil {
		return false
	}
	t.next = &singlyNode{value: value, next: t.next}
	if l.tail == t {
		l.tail = t.next
	}
	l.size++
	return true
}

// InsertBefore вставляет value перед первым вхождением target.
func (l *SinglyLinkedList) InsertBefore(target, value string) bool {
	if l.head == nil {
		return false
	}
	if l.head.value == target {
		l.PushFront(value)
		return true
	}
	for n := l.head; n.next != nil; n = n.next {
		if n.next.value == target {
			n.next = &singlyNode{value: value, next: n.next}
			l.size++
			return true
		}
	}
	return false
}

func (l *SinglyLinkedList) PopFront() (string, error) {
	if l.head == nil {
		return "", ErrEmpty
	}
	v := l.head.value
	l.head = l.head.next
	if l.head == nil {
		l.tail = nil
	}
	l.size--
	return v, nil
}

func (l *SinglyLinkedList) PopBack() (string, error) {
	if l.head == nil {
		return "", ErrEmpty
	}
	if l.head == l.tail {
		return l.PopFront()
	}
	prev := l.head
	for prev.next != l.tail {
		prev = prev.next
	}
	v := l.tail.value
	prev.next = nil
	l.tail = prev
	l.size--
	return v, nil
}

// RemoveByValue удаляет первое вхождение value.
func (l *SinglyLinkedList) RemoveByValue(v string) bool {
	if l.head == nil {
		return false
	}
	if l.head.value == v {
		_, _ = l.PopFront()
		return true
	}
	for n := l.head; n.next != nil; n = n.next {
		if n.next.value == v {
			victim := n.next
			n.next = victim.next
			if victim == l.tail {
				l.tail = n
			}
			l.size--
			return true
		}
	}
	return false
}

// RemoveAfter удаляет элемент, следующий за target.
func (l *SinglyLinkedList) RemoveAfter(target string) bool {
	t := l.find(target)
	if t == nil || t.next == nil {
		return false
	}
	victim := t.next
	t.next = victim.next
	if victim == l.tail {
		l.tail = t
	}
	l.size--
	return true
}

// RemoveBefore удаляет элемент, стоящий перед target.
func (l *SinglyLinkedList) RemoveBefore(target string) bool {
	if l.head == nil || l.head.value == target {
		return false
	}
	if l.head.next != nil && l.head.next.value == target {
		_, _ = l.PopFront()
		return true
	}
	for n := l.head; n.next != nil && n.next.next != nil; n = n.next {
		if n.next.next.value == target {
			n.next = n.next.next
			l.size--
			return true
		}
	}
	return false
}

func (l *SinglyLinkedList) GetAt(index int) (string, error) {
	if index < 0 || index >= l.size {
		return "", ErrIndex
	}
	n := l.head
	for i := 0; i < index; i++ {
		n = n.next
	}
	return n.value, nil
}

func (l *SinglyLinkedList) Front() (string, error) {
	if l.head == nil {
		return "", ErrEmpty
	}
	return l.head.value, nil
}

func (l *SinglyLinkedList) Back() (string, error) {
	if l.tail == nil {
		return "", ErrEmpty
	}
	return l.tail.value, nil
}

func (l *SinglyLinkedList) ToSlice() []string {
	out := make([]string, 0, l.size)
	for n := l.head; n != nil; n = n.next {
		out = append(out, n.value)
	}
	return out
}

func (l *SinglyLinkedList) Append(v string) error {
	l.PushBack(v)
	return nil
}
