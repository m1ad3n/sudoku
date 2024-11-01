#include "sudoku.h"

struct sudoku_t sudoku = {0};
struct termios oldattr;

int main(__unused int argc, __unused char* argv[]) {

	int
		row_pos = 0,
		col_pos = 0,
		input,
		quit = 0;

	sudoku_init(&oldattr);
	sudoku_new(&sudoku);

	while (!sudoku_completed(&sudoku) && !quit) {
		sudoku_display(&sudoku, row_pos, col_pos);

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
			if (sudoku_check_fields(&sudoku, (struct sudoku_pos){row_pos, col_pos}, input - '0') == SD_SAFE)
			{
				int safe = 1;
				for (int i = 0; i < sudoku.hard_count; i += 2) {
					if (row_pos == sudoku.hard_pos[i].row && col_pos == sudoku.hard_pos[i].col) {
						safe = 0;
						break;
					}
				}
				if (safe)
					sudoku.table[row_pos][col_pos] = input - '0';
			}
		}
		SD_CURSOR_UP(12);
		SD_CURSOR_LEFT(30);
	}

	putchar('\n');
	sudoku_end(&oldattr);
	
	return 0;
}
