#include <stdio.h>
#include <ctype.h>

#define MATRIX_WIDTH 10
#define MATRIX_DEPTH 22
#define TETROMINO_POSITIONS 4

enum tetromino_types
{
	tetromino_I,
	tetromino_J,
	tetromino_L,
	tetromino_O,
	tetromino_S,
	tetromino_T,
	tetromino_Z,
	illegal_tetromino = -1
};

enum square_state 
{
	empty = '.',
	red = 'r',
	green = 'g',
	blue = 'b',
	orange = 'o',
	cyan = 'c',
	magenta = 'm',
	yellow = 'y'
};

typedef struct tag_tetromino_pattern
{
	char *pattern[TETROMINO_POSITIONS];
	int width;
	int height;
} tetromino_pattern;

const tetromino_pattern tetromino_patterns[] = 
{
	{
		{
			"...."
			"cccc"
			"...."
			"....",
			"..c."
			"..c."
			"..c."
			"..c.",
			"...."
			"...."
			"cccc"
			"....",
			".c.."
			".c.."
			".c.."
			".c.."
		},
		4,
		4
	},
	{
		{
			"b.."
			"bbb"
			"...",
			".bb"
			".b."
			".b.",
			"..."
			"bbb"
			"..b",
			".b."
			".b."
			"bb."
		},
		3,
		3
	},
	{
		{
			"..o"
			"ooo"
			"...",
			".o."
			".o."
			".oo",
			"..."
			"ooo"
			"o..",
			"oo."
			".o."
			".o."
		},
		3,
		3
	},
	{
		{
			"yy"
			"yy",
			"yy"
			"yy",
			"yy"
			"yy",
			"yy"
			"yy"
		},
		2,
		2
	},
	{
		{
			".gg"
			"gg."
			"...",
			".g."
			".gg"
			"..g",
			"..."
			".gg"
			"gg.",
			"g.."
			"gg."
			".g."
		},
		3,
		3
	},
	{
		{
			".m."
			"mmm"
			"...",
			".m."
			".mm"
			".m.",
			"..."
			"mmm"
			".m.",
			".m."
			"mm."
			".m."
		},
		3,
		3
	},
	{
		{
			"rr."
			".rr"
			"...",
			"..r"
			".rr"
			".r.",
			"..."
			"rr."
			".rr",
			".r."
			"rr."
			"r.."
		},
		3,
		3
	}
};

typedef struct tag_tetromino
{
	int type;
	int position;
} tetromino;

char matrix[MATRIX_WIDTH][MATRIX_DEPTH];

int score;
int num_lines;
tetromino active_tetromino;

void init();
void clear_row(int row);
void clear_matrix();
void print_matrix();
bool check_square_value(char value);
void input_matrix();
void display_score();
void display_num_lines();
bool row_full(int row);
void exec_step();
void display_active_tetromino();
void rotate_right();
void rotate_left();

int main()
{
	bool in_game = true;
	bool in_command = false;
	int command;

	init();

	while (in_game)
	{
		command = getchar();

		if (in_command)
		{
			switch(command)
			{
			case 's':
				display_score();
				break;
			case 'n':
				display_num_lines();
				break;
			default:
				printf("unknown command %c\n", command);
				break;
			}

			in_command = false;
			continue;
		}

		if (command <= 0 || isspace(command))
		{
			continue;
		}

		switch (command)
		{
		case 'q':
			in_game = false;
			break;
		case 'p':
			print_matrix();
			break;
		case 'c':
			clear_matrix();
			break;
		case 'g':
			input_matrix();
			break;
		case 's':
			exec_step();
			break;
		case 't':
			display_active_tetromino();
			break;
		case 'I':
			active_tetromino.type = tetromino_I;
			break;
		case 'J':
			active_tetromino.type = tetromino_J;
			break;
		case 'L':
			active_tetromino.type = tetromino_L;
			break;
		case 'O':
			active_tetromino.type = tetromino_O;
			break;
		case 'S':
			active_tetromino.type = tetromino_S;
			break;
		case 'T':
			active_tetromino.type = tetromino_T;
			break;
		case 'Z':
			active_tetromino.type = tetromino_Z;
			break;
		case ')':
			rotate_right();
			break;
		case '(':
			rotate_left();
			break;
		case ';':
			putchar('\n');
			break;
		case '?':
			in_command = true;
			break;
		default:
			printf("unknown command %c\n", command);
			break;
		}
	}

	return 0;
}

void init()
{
	clear_matrix();

	score = 0;
	num_lines = 0;

	active_tetromino.type = illegal_tetromino;
	active_tetromino.position = 0;
}

void clear_row(int row)
{
	for (int col = 0; col < MATRIX_WIDTH; col++)
	{
		matrix[col][row] = empty;
	}
}

void clear_matrix()
{
	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		clear_row(row);
	}
}

void print_matrix()
{
	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		for (int col = 0; col < MATRIX_WIDTH; col++)
		{
			printf("%c ", matrix[col][row]);
		}
		putchar('\n');
	}
}

bool check_square_value(char value)
{
	return (value == empty || value == red || value == green
		|| value == blue || value == orange || value == cyan
		|| value == magenta || value == yellow);
}

void input_matrix()
{
	int value;

	for (int y = 0; y < MATRIX_DEPTH; y++)
	{
		for (int x = 0; x < MATRIX_WIDTH; )
		{
			value = getchar();

			if (value <= 0 || isspace(value))
			{
				continue;
			}

			matrix[x++][y] = check_square_value(value) ? value : empty;
		}
	}
}

void display_score()
{
	printf("%d\n", score);
}

void display_num_lines()
{
	printf("%d\n", num_lines);
}

bool row_full(int row)
{
	for (int col = 0; col < MATRIX_WIDTH; col++)
	{
		if (matrix[col][row] == empty)
		{
			return false;
		}
	}
	return true;
}

void exec_step()
{
	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		if (row_full(row))
		{
			clear_row(row);
			num_lines++;
			score += 100;
		}
	}
}

void display_active_tetromino()
{
	int tetromino_height;
	int tetromino_width;
	char *p_pattern;

	if (active_tetromino.type != illegal_tetromino)
	{
		tetromino_height = tetromino_patterns[active_tetromino.type].height;
		tetromino_width = tetromino_patterns[active_tetromino.type].width;
		p_pattern = tetromino_patterns[active_tetromino.type].pattern[active_tetromino.position];

		for (int row = 0; row < tetromino_height; row++)
		{
			for (int col = 0; col < tetromino_width; col++)
			{
				printf("%c ", *p_pattern++);
			}
			putchar('\n');
		}
	}
}

void rotate_right()
{
	active_tetromino.position++;
	if (active_tetromino.position > TETROMINO_POSITIONS - 1)
	{
		active_tetromino.position = 0;
	}
}

void rotate_left()
{
	active_tetromino.position--;
	if (active_tetromino.position < 0)
	{
		active_tetromino.position = TETROMINO_POSITIONS - 1;
	}
}
