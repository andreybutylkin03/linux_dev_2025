CFLAGS = -Wall -Wextra -Werror
TARGET = main main1 main2 main3
GENERATES = $(TARGET) main1.c main2.c main3.c
PATCHES = 1.patch 2.patch 3.patch

all: $(TARGET)

run: $(TARGET)
	./main
	./main1 8
	./main2 " @" 5
	./main3 42 ".#" 4

clean:
	rm -f $(TARGET) $(GENERATES) *.o

main: main.o
	$(CC) $(CFLAGS) $< -o $@

main1: main1.o
	$(CC) $(CFLAGS) $< -o $@

main2: main2.o
	$(CC) $(CFLAGS) $< -o $@

main3: main3.o
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main1.c: main.c
	patch main.c 1.patch -o main1.c

main2.c: main1.c
	patch main1.c 2.patch -o main2.c

main3.c: main2.c
	patch main2.c 3.patch -o main3.c
