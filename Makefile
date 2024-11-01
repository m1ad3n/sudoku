CC		:= gcc
CFLAGS	:= -g -Wall -Wextra -pedantic -s
SRCS	:= src/sudoku_cli.c src/sudoku.c
HDS		:= src/sudoku.h

sudoku.elf: $(SRCS) $(HDS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	@rm -rvf sudoku.elf
