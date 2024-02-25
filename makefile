CFLAGS=-Wextra -Werror -Wall -ggdb

main: main.c
	cc $(CFLAGS) -o main main.c -I./cutils
