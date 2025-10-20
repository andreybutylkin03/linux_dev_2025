CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = esub
SOURCE = esub.c
TEST_SCRIPT = test.sh

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET) *.o

test: $(TARGET)
	@echo "Running tests..."
	@bash $(TEST_SCRIPT)
