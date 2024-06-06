CC = gcc
CFLAGS = -c -Wall -Werror -Wextra
CFLAGS_WC = -Wall -Werror -Wextra
all: s21_grep clang test

s21_grep: 
	$(CC) $(CFLAGS) s21_grep.c flags_lib.c
	$(CC) $(CFLAGS_WC) s21_grep.o flags_lib.o -o s21_grep
	rm *.o

clang:
	clang-format -n *.c *.h

test:
	sh tests/func_test_forgrep.sh
	sh tests/leak_test_forgrep.sh

clean:
	rm s21_grep

rebuild: clean all