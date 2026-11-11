CXX ?= c++
CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -pedantic

.PHONY: check preview open
check: build/animation-test preview
	./build/animation-test
	python3 -m unittest discover -s tests -p 'test_*.py'
	node --check preview/preview.js
	node tests/preview_test.js

build/animation-test: tests/animation_test.cpp firmware/PocketEyes/Eyes.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -O1 -fsanitize=undefined -g $< -o $@

build/export-preview: tools/export_preview.cpp firmware/PocketEyes/Eyes.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -O2 $< -o $@

preview: build/export-preview
	./build/export-preview > preview/frames.js

open: preview
	open preview/index.html
