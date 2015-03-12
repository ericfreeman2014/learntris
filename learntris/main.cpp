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

typedef struct tag_matrix
{
	char squares[MATRIX_WIDTH][MATRIX_DEPTH];
} matrix;

typedef struct tag_game_state
{
	matrix main_matrix;
	tetromino active_tetromino;
	int score;
	int num_lines;
} game_state;

void init(game_state *this_game_state);
void clear_row(matrix *this_matrix, int row);
void clear_matrix(matrix *this_matrix);
void print_matrix(matrix *this_matrix);
bool check_square_value(char value);
void input_matrix(matrix *this_matrix);
void display_score(game_state *this_game_state);
void display_num_lines(game_state *this_game_state);
bool row_full(matrix *this_matrix, int row);
void exec_step(game_state *this_game_state);
void display_tetromino(tetromino *this_tetromino);
void rotate_right(tetromino *this_tetromino);
void rotate_left(tetromino *this_tetromino);

int main()
{
	bool in_game = true;
	bool in_command = false;
	int command;

	game_state my_game_state;
	matrix *main_matrix = &(my_game_state.main_matrix);
	tetromino *active_tetromino = &(my_game_state.active_tetromino);

	init(&my_game_state);

	while (in_game)
	{
		command = getchar();

		if (in_command)
		{
			switch(command)
			{
			case 's':
				display_score(&my_game_state);
				break;
			case 'n':
				display_num_lines(&my_game_state);
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
			print_matrix(main_matrix);
			break;
		case 'P':
			// print main matrix AND move matrix
			printf("command not implemented\n");
			break;
		case 'c':
			clear_matrix(main_matrix);
			break;
		case 'g':
			input_matrix(main_matrix);
			break;
		case 's':
			exec_step(&my_game_state);
			break;
		case 't':
			display_tetromino(active_tetromino);
			break;
		case 'I':
			active_tetromino->type = tetromino_I;
			break;
		case 'J':
			active_tetromino->type = tetromino_J;
			break;
		case 'L':
			active_tetromino->type = tetromino_L;
			break;
		case 'O':
			active_tetromino->type = tetromino_O;
			break;
		case 'S':
			active_tetromino->type = tetromino_S;
			break;
		case 'T':
			active_tetromino->type = tetromino_T;
			break;
		case 'Z':
			active_tetromino->type = tetromino_Z;
			break;
		case ')':
			rotate_right(active_tetromino);
			break;
		case '(':
			rotate_left(active_tetromino);
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

void init(game_state *this_game_state)
{
	clear_matrix(&(this_game_state->main_matrix));

	this_game_state->score = 0;
	this_game_state->num_lines = 0;

	this_game_state->active_tetromino.type = illegal_tetromino;
	this_game_state->active_tetromino.position = 0;
}

void clear_row(matrix *this_matrix, int row)
{
	for (int col = 0; col < MATRIX_WIDTH; col++)
	{
		this_matrix->squares[col][row] = empty;
	}
}

void clear_matrix(matrix *this_matrix)
{
	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		clear_row(this_matrix, row);
	}
}

void print_matrix(matrix *this_matrix)
{
	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		for (int col = 0; col < MATRIX_WIDTH; col++)
		{
			printf("%c ", this_matrix->squares[col][row]);
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

void input_matrix(matrix *this_matrix)
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

			this_matrix->squares[x++][y] = check_square_value(value) ? value : empty;
		}
	}
}

void display_score(game_state *this_game_state)
{
	printf("%d\n", this_game_state->score);
}

void display_num_lines(game_state *this_game_state)
{
	printf("%d\n", this_game_state->num_lines);
}

bool row_full(matrix *this_matrix, int row)
{
	for (int col = 0; col < MATRIX_WIDTH; col++)
	{
		if (this_matrix->squares[col][row] == empty)
		{
			return false;
		}
	}
	return true;
}

void exec_step(game_state *this_game_state)
{
	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		if (row_full(&(this_game_state->main_matrix), row))
		{
			clear_row(&(this_game_state->main_matrix), row);
			this_game_state->num_lines++;
			this_game_state->score += 100;
		}
	}
}

void display_tetromino(tetromino *this_tetromino)
{
	int tetromino_height;
	int tetromino_width;
	const tetromino_pattern *cur_pattern;
	char *pattern;

	if (this_tetromino->type != illegal_tetromino)
	{
		cur_pattern = tetromino_patterns + this_tetromino->type;
		tetromino_height = cur_pattern->height;
		tetromino_width = cur_pattern->width;
		pattern = cur_pattern->pattern[this_tetromino->position];

		for (int row = 0; row < tetromino_height; row++)
		{
			for (int col = 0; col < tetromino_width; col++)
			{
				printf("%c ", *pattern++);
			}
			putchar('\n');
		}
	}
}

void rotate_right(tetromino *this_tetromino)
{
	this_tetromino->position++;
	if (this_tetromino->position > TETROMINO_POSITIONS - 1)
	{
		this_tetromino->position = 0;
	}
}

void rotate_left(tetromino *this_tetromino)
{
	this_tetromino->position--;
	if (this_tetromino->position < 0)
	{
		this_tetromino->position = TETROMINO_POSITIONS - 1;
	}
}
