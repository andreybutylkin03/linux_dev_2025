CC = gcc
CFLAGS =  -g -O0

all: range

range: range.c
	$(CC) $(CFLAGS) -o range range.c

test: gdb_test1.gdb gdb_test2.gdb range
	@echo "Test 1"
	@gdb --batch --quiet -x gdb_test1.gdb range
	@echo ""
	@echo "Test 2"
	@gdb --batch --quiet -x gdb_test2.gdb range

clean:
	rm -rf range *.dSYM

.PHONY: all test clean

