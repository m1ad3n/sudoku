#ifndef SUDOKU_HEADER
#define SUDOKU_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#define __unused	__attribute__((unused))
#define __hot		__attribute__((hot))

#define BOX_HORIZONTAL		"\u2500"
#define BOX_VERTICAL		"\u2502"
#define BOX_LEFT_UP			"\u250C"
#define BOX_LEFT_DOWN		"\u2514"
#define BOX_LEFT_TO_RIGHT	"\u251C"
#define BOX_RIGHT_UP		"\u2510"
#define BOX_RIGHT_DOWN		"\u2518"
#define BOX_RIGHT_TO_LEFT	"\u2524"
#define BOX_MIDDLE_X		"\u253C"
#define BOX_TOP_TO_DOWN		"\u252C"
#define BOX_DOWN_TO_TOP		"\u2534"


#define SD_HIDE_CURSOR	fputs("\033[?25l", stdout);
#define SD_SHOW_CURSOR	fputs("\033[?25h", stdout);

#define SD_CURSOR_UP(x) printf("\033[%dF", x);
#define SD_CURSOR_LEFT(x) printf("\033[%dD", x);

#define SD_SIZE 		9
#define SD_SAFE 		0
#define SD_NOT_SAFE 	1

#define SD_QUIT			113
#define SD_UP			119
#define SD_DOWN			115
#define SD_RIGHT		100
#define SD_LEFT			97

struct sudoku_pos {
	int row;
	int col;
};

struct sudoku_t {
	int table[SD_SIZE][SD_SIZE];
	struct sudoku_pos hard_pos[40];
	int hard_count;
};

int sudoku_rand(int rmin, int rmax);
int sudoku_getch(void);
int sudoku_completed(const struct sudoku_t* sudoku);
int sudoku_check_fields(const struct sudoku_t* sudoku, struct sudoku_pos pos, int num);
void sudoku_init(struct termios* old);
void sudoku_new(struct sudoku_t* sudoku);
void sudoku_display(const struct sudoku_t* sudoku, int crow, int ccol);
void sudoku_end(struct termios* old);


#endif