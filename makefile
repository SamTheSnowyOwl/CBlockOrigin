CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -g3 -Iinclude
RM = del /q /f

all: Block

Block: src/main.o src/trie.o
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) Block.exe
	$(RM) src\*.o

.PHONY: all clean