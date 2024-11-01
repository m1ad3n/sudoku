#include "sudoku.h"

void sudoku_init(struct termios* old) {
	// for better random seed
	// we will use nano seconds of the real time
	struct timespec res;
	clock_gettime(CLOCK_REALTIME, &res);
	srand(res.tv_nsec);

	// setting the terminal pref
	// for getch function
	// so we don't need to press enter
	// after every key input
	struct termios newattr;
	tcgetattr( STDIN_FILENO, old );
	newattr = *old;
	newattr.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );

	SD_HIDE_CURSOR;
}

void sudoku_end(struct termios* old) {
	tcsetattr(STDIN_FILENO, TCSANOW, old);
	SD_SHOW_CURSOR;
}

int sudoku_rand(int rmin, int rmax) {
	return rand() % (rmax - rmin + 1) + rmin;
}

__hot int sudoku_getch(void) {
	int ch = getchar();
	if ((ch >= '0' && ch <= '9') ||
			(ch >= SD_LEFT && ch <= SD_UP)) {
		return ch;
	}
	return sudoku_getch();
}

void unicode(char* code) {
	fputs(code, stdout);
}

void sudoku_new(struct sudoku_t* sudoku) {
	for (int i = 0; i < SD_SIZE; i++) {
		for (int j = 0; j < SD_SIZE; j++) {
			sudoku->table[i][j] = 0;
		}
	}

	struct sudoku_pos pos;
	for (int row = 0; row < SD_SIZE; row++) {
		int n = sudoku_rand(1, 4);
		for (int j = 0; j < n; j++) {

			int col = sudoku_rand(0, 8);
			int value = sudoku_rand(1, 9);
			pos.row = row;
			pos.col = col;

			if (sudoku_check_fields(sudoku, pos, value) == SD_NOT_SAFE) {
				j--;
				continue;
			}

			sudoku->table[row][col] = value;
			sudoku->hard_pos[sudoku->hard_count++] = pos;
		}
	}
}

int sudoku_completed(const struct sudoku_t* sudoku) {
	for (int row = 0; row < SD_SIZE; row++) {
		for (int col = 0; col < SD_SIZE; col++) {
			if (sudoku->table[row][col] == 0) return 0;
		}
	}
	return 1;
}

__hot void sudoku_display(const struct sudoku_t* sudoku, int crow, int ccol) {
	int lnw = 23;

	unicode(BOX_LEFT_UP);
	for (int i = 0; i < lnw; i++) {
		if (i == 7 || i == 15) unicode(BOX_TOP_TO_DOWN);
		else unicode(BOX_HORIZONTAL);
	}
	unicode(BOX_RIGHT_UP);
	putchar(10);

	int i, j;
	for (i = 0; i < SD_SIZE; i++) {
		if (i % 3 == 0 && i != 0) {
			unicode(BOX_LEFT_TO_RIGHT);
			for (int i = 0; i < lnw; i++) {
				if (i == 7 || i == 15) unicode(BOX_MIDDLE_X);
				else unicode(BOX_HORIZONTAL);
			}
			puts(BOX_RIGHT_TO_LEFT);
		}

		// begin of the new row (line)
		unicode(BOX_VERTICAL);

		// cycle for every col
		for (j = 0; j < SD_SIZE; j++) {
			putchar(32); // space
			
			// for every third col put vr char
			if (j % 3 == 0 && j != 0) {
				unicode(BOX_VERTICAL);
				putchar(32);
			}
			
			// cursor pos, set background
			if (i == crow && j == ccol) {
				printf("\033[7m%d\033[0m", sudoku->table[i][j]);
				continue;
			}

			// every other number
			// check if its a hard value or not
			bool hard_value = 0;
			for (int hard_i = 0; hard_i < sudoku->hard_count; hard_i++) {
				if (i == sudoku->hard_pos[hard_i].row && j == sudoku->hard_pos[hard_i].col) {
					hard_value = 1;
					break;
				}
			}

			if (hard_value) {
				printf("\033[1m%d\033[0m", sudoku->table[i][j]);
			}
			else {
				putchar('0' + sudoku->table[i][j]);
			}
		}
		putchar(32);
		puts(BOX_VERTICAL);
	}

	unicode(BOX_LEFT_DOWN);
	for (int i = 0; i < lnw; i++) {
		if (i == 7 || i == 15) unicode(BOX_DOWN_TO_TOP);
		else unicode(BOX_HORIZONTAL);
	}
	unicode(BOX_RIGHT_DOWN);
}

__hot int sudoku_check_fields(const struct sudoku_t* sudoku, struct sudoku_pos pos, int num) {

	for (int i = 0; i < SD_SIZE; i++)
		if (sudoku->table[pos.row][i] == num)
			return SD_NOT_SAFE;

	for (int i = 0; i < SD_SIZE; i++)
		if (sudoku->table[i][pos.col] == num)
			return SD_NOT_SAFE;

	int block_row = pos.row - (pos.row % 3);
	int block_col = pos.col - (pos.col % 3);
	for (int block = 0; block < 3; block++) {
		for (int field = 0; field < 3; field++) {
			if (sudoku->table[block + block_row][field + block_col] == num)
				return SD_NOT_SAFE;
		}
	}

	return SD_SAFE;
}
