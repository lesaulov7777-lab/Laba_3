package ds

// Stack - стек на основе односвязного списка (вершина - голова списка).
type Stack struct{ list SinglyLinkedList }

func (s *Stack) TypeName() string { return "Stack" }
func (s *Stack) Size() int        { return s.list.Size() }
func (s *Stack) Clear()           { s.list.Clear() }
func (s *Stack) Push(v string)    { s.list.PushFront(v) }
func (s *Stack) Pop() (string, error) {
	return s.list.PopFront()
}
func (s *Stack) Top() (string, error) { return s.list.Front() }

// ToSlice возвращает элементы от дна к вершине.
func (s *Stack) ToSlice() []string {
	items := s.list.ToSlice()
	for i, j := 0, len(items)-1; i < j; i, j = i+1, j-1 {
		items[i], items[j] = items[j], items[i]
	}
	return items
}

func (s *Stack) Append(v string) error {
	s.Push(v)
	return nil
}

// Queue - очередь FIFO на односвязном списке с хвостом: Push и Pop за O(1).
type Queue struct{ list SinglyLinkedList }

func (q *Queue) TypeName() string       { return "Queue" }
func (q *Queue) Size() int              { return q.list.Size() }
func (q *Queue) Clear()                 { q.list.Clear() }
func (q *Queue) Push(v string)          { q.list.PushBack(v) }
func (q *Queue) Pop() (string, error)   { return q.list.PopFront() }
func (q *Queue) Front() (string, error) { return q.list.Front() }
func (q *Queue) ToSlice() []string      { return q.list.ToSlice() }
func (q *Queue) Append(v string) error  { q.Push(v); return nil }
