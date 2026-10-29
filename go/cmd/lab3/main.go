// Демонстрация ЛР3 (Go): сохранение/загрузка всех структур в бинарном и текстовом форматах.
package main

import (
	"fmt"
	"os"
	"path/filepath"
	"reflect"
	"sort"

	"lab3/ds"
)

func sorted(c ds.Container) []string {
	s := c.ToSlice()
	sort.Strings(s)
	return s
}

func roundTrip(dir string, mk func() ds.Container) error {
	src := mk()
	for _, v := range []string{"10", "20", "30", "40", "50"} {
		if err := src.Append(v); err != nil {
			return err
		}
	}
	base := filepath.Join(dir, src.TypeName())
	for _, spec := range []struct {
		ext  string
		save func(ds.Container, *os.File) error
		load func(ds.Container, *os.File) error
	}{
		{".bin", func(c ds.Container, f *os.File) error { return ds.SaveBinary(c, f) }, func(c ds.Container, f *os.File) error { return ds.LoadBinary(c, f) }},
		{".txt", func(c ds.Container, f *os.File) error { return ds.SaveText(c, f) }, func(c ds.Container, f *os.File) error { return ds.LoadText(c, f) }},
	} {
		w, err := os.Create(base + spec.ext)
		if err != nil {
			return err
		}
		err = spec.save(src, w)
		_ = w.Close()
		if err != nil {
			return err
		}
		r, err := os.Open(base + spec.ext)
		if err != nil {
			return err
		}
		dst := mk()
		err = spec.load(dst, r)
		_ = r.Close()
		if err != nil {
			return err
		}
		ok := reflect.DeepEqual(sorted(dst), sorted(src))
		fmt.Printf("%s%s: %s round-trip ok=%v\n", src.TypeName(), spec.ext, ds.Format(src), ok)
		if !ok {
			return fmt.Errorf("%s%s: mismatch", src.TypeName(), spec.ext)
		}
	}
	return nil
}

func main() {
	dir := "."
	if len(os.Args) > 1 {
		dir = os.Args[1]
	}
	if err := os.MkdirAll(dir, 0o755); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(2)
	}
	makers := []func() ds.Container{
		func() ds.Container { return ds.NewMyArray(2) },
		func() ds.Container { return &ds.SinglyLinkedList{} },
		func() ds.Container { return &ds.DoublyLinkedList{} },
		func() ds.Container { return &ds.Queue{} },
		func() ds.Container { return &ds.Stack{} },
		func() ds.Container { return ds.NewHashTable(8) },
		func() ds.Container { return &ds.CompleteBinaryTree{} },
	}
	for _, mk := range makers {
		if err := roundTrip(dir, mk); err != nil {
			fmt.Fprintln(os.Stderr, "error:", err)
			os.Exit(1)
		}
	}
}
