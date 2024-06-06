CC = gcc
CFLAGS = -c -Wall -Werror -Wextra
CFLAGS_WC = -Wall -Werror -Wextra
all: s21_cat

s21_cat: 
	$(CC) $(CFLAGS) s21_cat.c
	$(CC) $(CFLAGS_WC) s21_cat.o -o s21_cat
	rm *.o

clang:
	clang-format -n *.c *.h

test:
	sh tests/test_cat.sh

clean:
	rm s21_cat
	rm tests/log.txt

rebuild: clean all