# make build | test | bench | lint | coverage | run | clean
BUILD = build
COV   = build-cov
TIDY ?= /opt/homebrew/opt/llvm/bin/clang-tidy

build:
	cmake -S . -B $(BUILD) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD) -j

test: build
	$(BUILD)/lab3_tests

bench: build
	$(BUILD)/lab3_bench

run: build
	mkdir -p out && $(BUILD)/lab3 out

lint:
	cmake -S . -B $(BUILD) >/dev/null
	$(TIDY) -p $(BUILD) --extra-arg=-isysroot$$(xcrun --show-sdk-path) --extra-arg=-I$$(xcrun --show-sdk-path)/usr/include/c++/v1 src/*.cpp app/*.cpp

coverage:
	cmake -S . -B $(COV) -DCOVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(COV) -j
	lcov --zerocounters --directory $(COV) -q
	$(COV)/lab3_tests
	lcov --capture --directory $(COV) --output-file $(COV)/all.info --ignore-errors inconsistent,mismatch,unused,gcov,format -q
	lcov --extract $(COV)/all.info '*/lab3/cpp/src/*' --output-file $(COV)/lab3.info --ignore-errors unused,inconsistent,format -q
	lcov --summary $(COV)/lab3.info --ignore-errors inconsistent,format
	genhtml $(COV)/lab3.info --output-directory coverage-html --ignore-errors inconsistent,format -q
	@echo "HTML-отчёт: coverage-html/index.html"

clean:
	rm -rf $(BUILD) $(COV) coverage-html out

.PHONY: build test bench run lint coverage clean
