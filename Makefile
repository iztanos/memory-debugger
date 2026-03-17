CC = gcc

CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = record_test

SRC = src/test.c


all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)


clean:
	rm -f $(TARGET)