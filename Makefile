CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
TARGET = memdebug_demo
TEST_TARGET = memdebug_test

SRC = src/main.c src/list.c src/memdebug.c
OBJ = $(SRC:.c=.o)
TEST_SRC = src/test.c src/list.c src/memdebug.c
TEST_OBJ = $(TEST_SRC:.c=.test.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

src/%.o: src/%.c include/memdebug.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) -o $(TEST_TARGET)

src/%.test.o: src/%.c include/memdebug.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET)
