package ds

// Djb2 - хеш-функция DJB2 (h = h*33 + c).
func Djb2(key string) uint64 {
	h := uint64(5381)
	for i := 0; i < len(key); i++ {
		h = h*33 + uint64(key[i])
	}
	return h
}

// HashTable - множество строк на цепочках; расширяется при заполнении > 0.75.
type HashTable struct {
	buckets []SinglyLinkedList
	size    int
}

// NewHashTable создаёт таблицу с заданным числом корзин (минимум 1).
func NewHashTable(buckets int) *HashTable {
	if buckets < 1 {
		buckets = 1
	}
	return &HashTable{buckets: make([]SinglyLinkedList, buckets)}
}

func (h *HashTable) TypeName() string { return "HashTable" }
func (h *HashTable) Size() int        { return h.size }
func (h *HashTable) BucketCount() int { return len(h.buckets) }

func (h *HashTable) idx(key string, n int) int { return int(Djb2(key) % uint64(n)) }

func (h *HashTable) Contains(key string) bool {
	return h.buckets[h.idx(key, len(h.buckets))].Contains(key)
}

// Insert возвращает false, если ключ уже есть.
func (h *HashTable) Insert(key string) bool {
	if h.Contains(key) {
		return false
	}
	if (h.size+1)*4 > len(h.buckets)*3 {
		h.rehash(len(h.buckets) * 2)
	}
	h.buckets[h.idx(key, len(h.buckets))].PushFront(key)
	h.size++
	return true
}

func (h *HashTable) Erase(key string) bool {
	if !h.buckets[h.idx(key, len(h.buckets))].RemoveByValue(key) {
		return false
	}
	h.size--
	return true
}

func (h *HashTable) rehash(n int) {
	fresh := make([]SinglyLinkedList, n)
	for i := range h.buckets {
		for _, k := range h.buckets[i].ToSlice() {
			fresh[h.idx(k, n)].PushFront(k)
		}
	}
	h.buckets = fresh
}

// MaxChainLength - длина самой длинной цепочки.
func (h *HashTable) MaxChainLength() int {
	m := 0
	for i := range h.buckets {
		if s := h.buckets[i].Size(); s > m {
			m = s
		}
	}
	return m
}

func (h *HashTable) ToSlice() []string {
	out := make([]string, 0, h.size)
	for i := range h.buckets {
		out = append(out, h.buckets[i].ToSlice()...)
	}
	return out
}

func (h *HashTable) Clear() {
	for i := range h.buckets {
		h.buckets[i].Clear()
	}
	h.size = 0
}

func (h *HashTable) Append(v string) error {
	h.Insert(v)
	return nil
}
