// Package ds содержит структуры данных ЛР3 (Go-версия) и их сериализацию.
//
// Форматы файлов совпадают с C++-версией:
//
//	бинарный: "LR3B" | u8 len(typeName) | typeName | u32 count | count * (u32 len | bytes)  (little-endian)
//	текстовый: "LR3T <typeName> <count>\n" и по одной строке на элемент (\\ \n \r экранируются)
package ds

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"strconv"
	"strings"
)

// ErrEmpty возвращается при чтении из пустой структуры.
var ErrEmpty = errors.New("ds: container is empty")

// ErrIndex возвращается при обращении по индексу вне диапазона.
var ErrIndex = errors.New("ds: index out of range")

const (
	magicBin  = "LR3B"
	magicText = "LR3T"
	maxLen    = 64 * 1024 * 1024
)

// Container - общий интерфейс всех структур; сериализация строится на его пяти методах.
type Container interface {
	TypeName() string
	Size() int
	ToSlice() []string // порядок, достаточный для восстановления
	Clear()
	Append(value string) error // добавление при десериализации
}

// SaveBinary записывает контейнер в бинарном формате.
func SaveBinary(c Container, w io.Writer) error {
	name := c.TypeName()
	items := c.ToSlice()
	var buf bytes.Buffer
	buf.WriteString(magicBin)
	buf.WriteByte(byte(len(name)))
	buf.WriteString(name)
	var u [4]byte
	binary.LittleEndian.PutUint32(u[:], uint32(len(items)))
	buf.Write(u[:])
	for _, s := range items {
		binary.LittleEndian.PutUint32(u[:], uint32(len(s)))
		buf.Write(u[:])
		buf.WriteString(s)
	}
	_, err := w.Write(buf.Bytes())
	return err
}

// LoadBinary заменяет содержимое контейнера данными из бинарного потока.
// При ошибке контейнер не изменяется.
func LoadBinary(c Container, r io.Reader) error {
	magic := make([]byte, 4)
	if _, err := io.ReadFull(r, magic); err != nil || string(magic) != magicBin {
		return errors.New("ds: bad binary magic")
	}
	var nameLen [1]byte
	if _, err := io.ReadFull(r, nameLen[:]); err != nil {
		return fmt.Errorf("ds: unexpected end of binary data: %w", err)
	}
	name := make([]byte, nameLen[0])
	if _, err := io.ReadFull(r, name); err != nil {
		return fmt.Errorf("ds: unexpected end of binary data: %w", err)
	}
	if string(name) != c.TypeName() {
		return fmt.Errorf("ds: type mismatch: file has %s, expected %s", name, c.TypeName())
	}
	count, err := readU32(r)
	if err != nil {
		return err
	}
	var items []string
	for i := uint32(0); i < count; i++ {
		n, err := readU32(r)
		if err != nil {
			return err
		}
		if n > maxLen {
			return errors.New("ds: element too large")
		}
		b := make([]byte, n)
		if _, err := io.ReadFull(r, b); err != nil {
			return fmt.Errorf("ds: unexpected end of binary data: %w", err)
		}
		items = append(items, string(b))
	}
	return replace(c, items)
}

func readU32(r io.Reader) (uint32, error) {
	var b [4]byte
	if _, err := io.ReadFull(r, b[:]); err != nil {
		return 0, fmt.Errorf("ds: unexpected end of binary data: %w", err)
	}
	return binary.LittleEndian.Uint32(b[:]), nil
}

// SaveText записывает контейнер в текстовом формате.
func SaveText(c Container, w io.Writer) error {
	items := c.ToSlice()
	var sb strings.Builder
	fmt.Fprintf(&sb, "%s %s %d\n", magicText, c.TypeName(), len(items))
	for _, s := range items {
		sb.WriteString(escape(s))
		sb.WriteByte('\n')
	}
	_, err := io.WriteString(w, sb.String())
	return err
}

// LoadText заменяет содержимое контейнера данными из текстового потока.
func LoadText(c Container, r io.Reader) error {
	br := bufio.NewReader(r)
	header, err := br.ReadString('\n')
	if err != nil && header == "" {
		return errors.New("ds: bad text header")
	}
	fields := strings.Fields(header)
	if len(fields) != 3 || fields[0] != magicText {
		return errors.New("ds: bad text header")
	}
	if fields[1] != c.TypeName() {
		return fmt.Errorf("ds: type mismatch: file has %s, expected %s", fields[1], c.TypeName())
	}
	count, err := strconv.Atoi(fields[2])
	if err != nil || count < 0 {
		return errors.New("ds: bad text header")
	}
	var items []string
	for i := 0; i < count; i++ {
		line, err := br.ReadString('\n')
		if err != nil {
			return errors.New("ds: unexpected end of text data")
		}
		s, err := unescape(strings.TrimSuffix(line, "\n"))
		if err != nil {
			return err
		}
		items = append(items, s)
	}
	return replace(c, items)
}

// replace атомарно (с откатом) подменяет содержимое контейнера.
func replace(c Container, items []string) error {
	backup := c.ToSlice()
	c.Clear()
	for _, s := range items {
		if err := c.Append(s); err != nil {
			c.Clear()
			for _, old := range backup {
				_ = c.Append(old)
			}
			return err
		}
	}
	return nil
}

func escape(s string) string {
	r := strings.NewReplacer(`\`, `\\`, "\n", `\n`, "\r", `\r`)
	return r.Replace(s)
}

func unescape(s string) (string, error) {
	var sb strings.Builder
	for i := 0; i < len(s); i++ {
		if s[i] != '\\' {
			sb.WriteByte(s[i])
			continue
		}
		i++
		if i >= len(s) {
			return "", errors.New("ds: dangling escape in text data")
		}
		switch s[i] {
		case '\\':
			sb.WriteByte('\\')
		case 'n':
			sb.WriteByte('\n')
		case 'r':
			sb.WriteByte('\r')
		default:
			return "", errors.New("ds: unknown escape in text data")
		}
	}
	return sb.String(), nil
}

// Format выводит содержимое в виде "[a, b, c]".
func Format(c Container) string {
	return "[" + strings.Join(c.ToSlice(), ", ") + "]"
}
