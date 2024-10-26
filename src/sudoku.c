#include "sudoku.h"

void sudoku_init(sudoku_t* sudoku, struct termios* old);
int sudoku_rand(int rmin, int rmax);
int sudoku_getch(void);
int sudoku_getpos(int *x, int *y);
void sudoku_new(sudoku_t sudoku, int hard_pos[40], int* hard_index);
void sudoku_display(sudoku_t sudoku, int row, int col, int hard_pos[40]);
int sudoku_check_fields(sudoku_t sudoku, int row, int col, int num);
int sudoku_completed(sudoku_t sudoku);
void sudoku_end(struct termios* old);

int main(__unused int argc, __unused char* argv[]) {

	while (1) {
		if (sudoku_rand(0, 8) < 0) return 1;
	}

	int
		xpos,
		ypos,
		row_pos = 0,
		col_pos = 0,
		input,
		quit = 0;

	sudoku_t sudoku = {0};
	struct termios oldattr;

	int hard_pos[40] = {0};
	int hard_index = 0;

	sudoku_init(&sudoku, &oldattr);
	sudoku_new(sudoku, hard_pos, &hard_index);
	sudoku_getpos(&ypos, &xpos);

	while (!sudoku_completed(sudoku) && !quit) {
		sudoku_display(sudoku, row_pos, col_pos, hard_pos);
		input = sudoku_getch();
		switch (input) {
		case SD_UP:
			if (row_pos) row_pos--;
			break;

		case SD_DOWN:
			if (row_pos < 8) row_pos++;
			break;

		case SD_RIGHT:
			if (col_pos < 8) col_pos++;
			break;

		case SD_LEFT:
			if (col_pos > 0) col_pos--;
			break;

		case SD_QUIT:
			quit = 1;
			break;

		default:
			if (sudoku_check_fields(sudoku, row_pos, col_pos, input - '0') == SD_SAFE) {
				int safe = 1;
				for (int i = 0; i < 40; i += 2) {
					if (row_pos == hard_pos[i] && col_pos == hard_pos[i + 1]) {
						safe = 0;
						break;
					}
				}
				if (safe)
					sudoku[row_pos][col_pos] = input - '0';
			}
		}
		if (!quit) {
			printf("\033[%d;%dH", xpos, ypos);
		}
		else {
			putchar(10);
		}
	}

	sudoku_end(&oldattr);
	return 0;
}

int sudoku_getpos(int *x, int *y) {

	char buf[30] = {0};
	int ret, i, pow;
	char ch;

	*x = 0; *y = 0;

	write(1, "\033[6n", 4);

	for( i = 0, ch = 0; ch != 'R'; i++ ) {
	  ret = read(0, &ch, 1);
	  if ( !ret ) {
   		return 1;
	  }
	  buf[i] = ch;
	}

	if (i < 2) {
	  printf("i < 2\n");
	  return 1;
	}

	for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
	  *x = *x + ( buf[i] - '0' ) * pow;

	for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
	  *y = *y + ( buf[i] - '0' ) * pow;

	return 0;
}

void sudoku_init(sudoku_t* sudoku, struct termios* old) {
	// for better random seed
	// we will use nano seconds of real time
	struct timespec res;
	clock_gettime(CLOCK_REALTIME, &res);
	srand(res.tv_nsec);

	for (int i = 0; i < SD_SIZE; i++) {
		for (int j = 0; j < SD_SIZE; j++) {
			*sudoku[i][j] = 0;
		}
	}

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

void sudoku_new(sudoku_t sudoku, int hard_pos[40], int* hard_index) {
	for (int i = 0; i < SD_SIZE; i++) {
		int n = sudoku_rand(1, 4);
		for (int j = 0; j < n; j++) {
			int pos = sudoku_rand(0, 8);
			int value = sudoku_rand(1, 9);

			if (sudoku_check_fields(sudoku, i, pos, value) == SD_NOT_SAFE) {
				j--;
				continue;
			}

			printf("VALUE: %d\tROW: %d\tCOL: %d\n", value, i, pos);

			sudoku[i][pos] = value;
			hard_pos[*hard_index] = i;
			hard_pos[*hard_index + 1] = pos;
			*hard_index += 2;
		}
	}
}

int sudoku_completed(sudoku_t sudoku) {
	for (int row = 0; row < SD_SIZE; row++) {
		for (int col = 0; col < SD_SIZE; col++) {
			if (sudoku[row][col] == 0) return 0;
		}
	}
	return 1;
}

__hot void sudoku_display(sudoku_t sudoku, int row, int col, int hard_pos[40]) {
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

		unicode(BOX_VERTICAL);
		for (j = 0; j < SD_SIZE; j++) {
			putchar(32);
			if (j % 3 == 0 && j != 0) {
				unicode(BOX_VERTICAL);
				putchar(32);
			}
			if (i == row && j == col) {
				printf("\033[7m%d\033[0m", sudoku[i][j]);
			}
			else {
				bool hard_value = 0;
				for (int hard_i = 0; hard_i < 40; hard_i += 2) {
					if (i == hard_pos[hard_i] && j == hard_pos[hard_i + 1]) {
						hard_value = 1;
						break;
					}
				}

				if (hard_value) {
					printf("\033[1m%d\033[0m", sudoku[i][j]);
				}
				else {
					putchar('0' + sudoku[i][j]);
				}
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

__hot int sudoku_check_fields(sudoku_t sudoku, int row, int col, int num) {

	for (int i = 0; i < SD_SIZE; i++)
		if (sudoku[row][i] == num)
			return SD_NOT_SAFE;

	for (int i = 0; i < SD_SIZE; i++)
		if (sudoku[i][col] == num)
			return SD_NOT_SAFE;

	int block_row = row - (row % 3);
	int block_col = col - (col % 3);
	for (int block = 0; block < 3; block++) {
		for (int field = 0; field < 3; field++) {
			if (sudoku[block + block_row][field + block_col] == num)
				return SD_NOT_SAFE;
		}
	}

	return SD_SAFE;
}
