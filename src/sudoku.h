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

#define SD_SIZE 		9
#define SD_SAFE 		0
#define SD_NOT_SAFE 	1

#define SD_QUIT			113
#define SD_UP			119
#define SD_DOWN			115
#define SD_RIGHT		100
#define SD_LEFT			97

typedef int sudoku_t[SD_SIZE][SD_SIZE];

#endif