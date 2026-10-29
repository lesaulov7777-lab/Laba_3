# make test | cover | bench | lint | run | clean
test:
	go test ./...

cover:
	go test ./ds -coverprofile=coverage.out
	go tool cover -func=coverage.out | tail -1
	go tool cover -html=coverage.out -o coverage.html
	@echo "HTML-отчёт: coverage.html"

bench:
	go test ./ds -run '^$$' -bench . -benchmem

lint:
	gofmt -l . | (! grep .)
	go vet ./...

run:
	go run ./cmd/lab3 out

clean:
	rm -rf coverage.out coverage.html out

.PHONY: test cover bench lint run clean
