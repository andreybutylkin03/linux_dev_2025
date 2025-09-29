CC = gcc
CFLAGS = -Wall -fPIC -O2
LDFLAGS =
LDLIBS =

GENERATES = prog prog-a prog-so liboutput_static.a liboutput.so *.na *.oa *.ta README
TRASH = *.o *~ o.* *.na *.oa *.ta prog prog-a prog-so liboutput_static.a liboutput.so

.PHONY: all clean distclean test

all: prog prog-a prog-so liboutput_static.a liboutput.so README

liboutput_static.a: fun.o const.o
	$(AR) rcs $@ $^

liboutput.so: fun.o const.o
	$(CC) -shared -o $@ $^

prog: prog.o fun.o const.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

prog-a: prog.o liboutput_static.a
	$(CC) -o $@ $< -L. -loutput_static

prog-so: prog.o liboutput.so
	$(CC) -o $@ $< -L. -loutput -Wl,-rpath,.

README: prog
	./$< 2> $@

%.o: %.c outlib.h
	$(CC) $(CFLAGS) -c $< -o $@

test: prog prog-a prog-so
	@echo "=== Запуск тестов ==="; \
	OUTFMT_NOARGS=".na"; OUTFMT_ONE=".oa"; OUTFMT_THREE=".ta"; REF=prog; \
	for bin in prog prog-a prog-so; do \
	    echo "Running $$bin..."; \
	    ./$$bin        > "$$bin$$OUTFMT_NOARGS" 2>&1 || true; \
	    ./$$bin arg > "$$bin$$OUTFMT_ONE"      2>&1 || true; \
	    ./$$bin arg1 arg2 arg3 > "$$bin$$OUTFMT_THREE" 2>&1 || true; \
	done; \
	echo "=== Сравнение вывода ==="; \
	for bin in prog-a prog-so; do \
	    cmp "$$REF$$OUTFMT_NOARGS"  "$$bin$$OUTFMT_NOARGS"  || echo "No args mismatch: $$bin"; \
	    cmp "$$REF$$OUTFMT_ONE"     "$$bin$$OUTFMT_ONE"     || echo "One arg mismatch:  $$bin"; \
	    cmp "$$REF$$OUTFMT_THREE"   "$$bin$$OUTFMT_THREE"   || echo "Three args mismatch: $$bin"; \
	done; \
	echo "Все тесты успешно пройдены!"; \

clean:
	rm -f $(TRASH)

distclean: clean
	rm -rf $(GENERATES)
