package ds

// MyArray - динамический массив строк с удвоением ёмкости.
type MyArray struct {
	data []string
	size int
}

// NewMyArray создаёт массив с начальной ёмкостью capacity (минимум 1).
func NewMyArray(capacity int) *MyArray {
	if capacity < 1 {
		capacity = 1
	}
	return &MyArray{data: make([]string, capacity)}
}

func (a *MyArray) TypeName() string { return "MyArray" }
func (a *MyArray) Size() int        { return a.size }
func (a *MyArray) Capacity() int    { return len(a.data) }
func (a *MyArray) Clear()           { a.size = 0 }

func (a *MyArray) grow() {
	if a.size < len(a.data) {
		return
	}
	n := len(a.data) * 2
	if n == 0 {
		n = 2
	}
	fresh := make([]string, n)
	copy(fresh, a.data[:a.size])
	a.data = fresh
}

// PushBack добавляет элемент в конец.
func (a *MyArray) PushBack(v string) {
	a.grow()
	a.data[a.size] = v
	a.size++
}

// InsertAt вставляет элемент по индексу (index == Size() допустим).
func (a *MyArray) InsertAt(index int, v string) error {
	if index < 0 || index > a.size {
		return ErrIndex
	}
	a.grow()
	copy(a.data[index+1:a.size+1], a.data[index:a.size])
	a.data[index] = v
	a.size++
	return nil
}

// GetAt возвращает элемент по индексу.
func (a *MyArray) GetAt(index int) (string, error) {
	if index < 0 || index >= a.size {
		return "", ErrIndex
	}
	return a.data[index], nil
}

// SetAt заменяет элемент по индексу.
func (a *MyArray) SetAt(index int, v string) error {
	if index < 0 || index >= a.size {
		return ErrIndex
	}
	a.data[index] = v
	return nil
}

// RemoveAt удаляет элемент по индексу.
func (a *MyArray) RemoveAt(index int) error {
	if index < 0 || index >= a.size {
		return ErrIndex
	}
	copy(a.data[index:], a.data[index+1:a.size])
	a.size--
	a.data[a.size] = ""
	return nil
}

func (a *MyArray) ToSlice() []string {
	out := make([]string, a.size)
	copy(out, a.data[:a.size])
	return out
}

func (a *MyArray) Append(v string) error {
	a.PushBack(v)
	return nil
}
