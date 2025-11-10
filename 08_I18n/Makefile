all: build

build:
	mkdir -p build && cd build && cmake .. && make

update:
	cd build && make update_po

clean:
	rm -rf build

run: build
	cd build && ./guess

install: build
	cd build && sudo make install

.PHONY: all build update clean run install
