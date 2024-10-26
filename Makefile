CC		:= gcc
CFLAGS	:= -g -Wall -Wextra -pedantic -s

sudoku.elf: src/sudoku.c src/sudoku.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@rm -rvf sudoku.elf
