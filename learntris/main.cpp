#include <stdio.h>
#include <ctype.h>
#include <string.h>

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

typedef struct tag_tetromino_location
{
	int top;
	int left;
} tetromino_location;

typedef struct tag_tetromino
{
	int type;
	int position;
	tetromino_location location;
} tetromino;

typedef struct tag_matrix
{
	char squares[MATRIX_WIDTH * MATRIX_DEPTH + 1];
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
void spawn_tetromino(tetromino *this_tetromino, int tetromino_type);
void display_tetromino(tetromino *this_tetromino);
bool rotate_right(tetromino *this_tetromino, matrix *this_matrix);
bool rotate_left(tetromino *this_tetromino, matrix *this_matrix);
bool check_collision_right(tetromino *this_tetromino, matrix *this_matrix, const tetromino_pattern *cur_pattern, int extra_move);
bool check_collision_left(tetromino *this_tetromino, matrix *this_matrix, const tetromino_pattern *cur_pattern, int extra_move);
bool check_collision_down(tetromino *this_tetromino, matrix *this_matrix, const tetromino_pattern *cur_pattern, int extra_move);
bool nudge_right(tetromino *this_tetromino, matrix *this_matrix);
bool nudge_left(tetromino *this_tetromino, matrix *this_matrix);
bool nudge_down(tetromino *this_tetromino, matrix *this_matrix);
int empty_right(const tetromino_pattern *this_pattern, int position);
int empty_left(const tetromino_pattern *this_pattern, int position);
int empty_top(const tetromino_pattern *this_pattern, int position);
int empty_bottom(const tetromino_pattern *this_pattern, int position);
void insert_tetromino(matrix *this_matrix, tetromino *new_tetromino);
void print_all(game_state *this_game_state);
void drop_tetromino(game_state *this_game_state);

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
			print_all(&my_game_state);
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
			spawn_tetromino(active_tetromino, tetromino_I);
			break;
		case 'J':
			spawn_tetromino(active_tetromino, tetromino_J);
			break;
		case 'L':
			spawn_tetromino(active_tetromino, tetromino_L);
			break;
		case 'O':
			spawn_tetromino(active_tetromino, tetromino_O);
			break;
		case 'S':
			spawn_tetromino(active_tetromino, tetromino_S);
			break;
		case 'T':
			spawn_tetromino(active_tetromino, tetromino_T);
			break;
		case 'Z':
			spawn_tetromino(active_tetromino, tetromino_Z);
			break;
		case ')':
			rotate_right(active_tetromino, main_matrix);
			break;
		case '(':
			rotate_left(active_tetromino, main_matrix);
			break;
		case '>':
			nudge_right(active_tetromino, main_matrix);
			break;
		case '<':
			nudge_left(active_tetromino, main_matrix);
			break;
		case 'v':
			nudge_down(active_tetromino, main_matrix);
			break;
		case 'V':
			drop_tetromino(&my_game_state);
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
	this_game_state->active_tetromino.position = -1;
	this_game_state->active_tetromino.location.top = -1;
	this_game_state->active_tetromino.location.left = -1;
}

void clear_row(matrix *this_matrix, int row)
{
	for (int col = 0; col < MATRIX_WIDTH; col++)
	{
		this_matrix->squares[MATRIX_WIDTH * row + col] = empty;
	}
}

void clear_matrix(matrix *this_matrix)
{
	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		clear_row(this_matrix, row);
	}
	this_matrix->squares[MATRIX_WIDTH * MATRIX_DEPTH] = '\0';
}

void print_matrix(matrix *this_matrix)
{
	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		for (int col = 0; col < MATRIX_WIDTH; col++)
		{
			printf("%c ", this_matrix->squares[MATRIX_WIDTH * row + col]);
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

	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		for (int col = 0; col < MATRIX_WIDTH; )
		{
			value = getchar();

			if (value <= 0 || isspace(value))
			{
				continue;
			}

			this_matrix->squares[MATRIX_WIDTH * row + col++] =
				check_square_value(value) ? value : empty;
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
		if (this_matrix->squares[MATRIX_WIDTH * row + col] == empty)
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

void spawn_tetromino(tetromino *this_tetromino, int tetromino_type)
{
	const tetromino_pattern *cur_pattern;

	this_tetromino->type = tetromino_type;
	cur_pattern = tetromino_patterns + this_tetromino->type;
	this_tetromino->position = 0;
	this_tetromino->location.top = 0;
	this_tetromino->location.left =
		MATRIX_WIDTH / 2 - (int)(cur_pattern->width / 2.0 + 0.5);
}

void display_tetromino(tetromino *this_tetromino)
{
	int tetromino_height;
	int tetromino_width;
	const tetromino_pattern *cur_pattern;
	char *pattern;

	if (this_tetromino->type == illegal_tetromino
		|| this_tetromino->position < 0)
	{
		return;
	}

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

bool rotate_right(tetromino *this_tetromino, matrix *this_matrix)
{
	if (this_tetromino->position < 0)
	{
		return false;
	}

	this_tetromino->position++;
	if (this_tetromino->position > TETROMINO_POSITIONS - 1)
	{
		this_tetromino->position = 0;
	}

	return true;

	// TO DO
	// if tetromino doesn't fit - move it away from the wall!!!
	// 1. check if current position allows for current left
	// 2. if not - check if we can move away from the wall
	// a) if we can move - move
	// b) if we cannot move - refuse to rotate
}

bool rotate_left(tetromino *this_tetromino, matrix *this_matrix)
{
	if (this_tetromino->position < 0)
	{
		return false;
	}

	this_tetromino->position--;
	if (this_tetromino->position < 0)
	{
		this_tetromino->position = TETROMINO_POSITIONS - 1;
	}

	return true;

	// TO DO
	// if tetromino doesn't fit - move it away from the wall!!!
	// 1. check if current position allows for current left
	// 2. if not - check if we can move away from the wall
	// a) if we can move - move
	// b) if we cannot move - refuse to rotate
}

bool check_collision_right(tetromino *this_tetromino, matrix *this_matrix, const tetromino_pattern *cur_pattern, int extra_move)
{
	return false;
	/*
	int tetramino_bitfield, matrix_bitfield, row, col;

	tetramino_bitfield = 0;
	col = cur_pattern->width - 1 - extra_move;
	for (row = 0; row < cur_pattern->height; row++)
	{
		char ch;
		int bit;
		ch = cur_pattern->pattern[this_tetromino->position]
								 [cur_pattern->height * row + col];
		bit = (ch == empty) ? 0 : 1;
		tetramino_bitfield += bit << row;
	}
	
	matrix_bitfield = 0;
	col = this_tetromino->location.left + cur_pattern->width - extra_move;
	for (row = this_tetromino->location.top;
		row < this_tetromino->location.top + cur_pattern->height; col++)
	{
		char ch;
		int bit;
		ch = this_matrix->squares[MATRIX_WIDTH * row + col];
		bit = (ch == empty) ? 0 : 1;
		matrix_bitfield += bit << (row - this_tetromino->location.top);
	}

	return ((tetramino_bitfield & matrix_bitfield) != 0);
	*/
}

bool check_collision_left(tetromino *this_tetromino, matrix *this_matrix, const tetromino_pattern *cur_pattern, int extra_move)
{
	return false;
	/*
	int tetramino_bitfield, matrix_bitfield, row, col;

	tetramino_bitfield = 0;
	col = 0 + extra_move;
	for (row = 0; row < cur_pattern->height; row++)
	{
		char ch;
		int bit;
		ch = cur_pattern->pattern[this_tetromino->position]
								 [cur_pattern->height * row + col];
		bit = (ch == empty) ? 0 : 1;
		tetramino_bitfield += bit << row;
	}
	
	matrix_bitfield = 0;
	col = this_tetromino->location.left - 1 + extra_move;
	for (row = this_tetromino->location.top;
		row < this_tetromino->location.top + cur_pattern->height; col++)
	{
		char ch;
		int bit;
		ch = this_matrix->squares[MATRIX_WIDTH * row + col];
		bit = (ch == empty) ? 0 : 1;
		matrix_bitfield += bit << (row - this_tetromino->location.top);
	}

	return ((tetramino_bitfield & matrix_bitfield) != 0);
	*/
}

bool check_collision_down(tetromino *this_tetromino, matrix *this_matrix, const tetromino_pattern *cur_pattern, int extra_move)
{
	int tetramino_bitfield, matrix_bitfield, row, col;

	tetramino_bitfield = 0;
	row = cur_pattern->height - 1 - extra_move;
	for (col = 0; col < cur_pattern->width; col++)
	{
		char ch;
		int bit;
		ch = cur_pattern->pattern[this_tetromino->position]
								 [cur_pattern->height * row + col];
		bit = (ch == empty) ? 0 : 1;
		tetramino_bitfield += bit << col;
	}
	
	matrix_bitfield = 0;
	row = this_tetromino->location.top + cur_pattern->height - extra_move;
	for (col = this_tetromino->location.left;
		col < this_tetromino->location.left + cur_pattern->width; col++)
	{
		char ch;
		int bit;
		ch = this_matrix->squares[MATRIX_WIDTH * row + col];
		bit = (ch == empty) ? 0 : 1;
		matrix_bitfield += bit << (col - this_tetromino->location.left);
	}

	return ((tetramino_bitfield & matrix_bitfield) != 0);
}

bool nudge_right(tetromino *this_tetromino, matrix *this_matrix)
{
	const tetromino_pattern *cur_pattern;
	int old_value;
	int extra_move;

	if (this_tetromino->location.left < 0)
	{
		return false;
	}

	old_value = this_tetromino->location.left;
	this_tetromino->location.left++;

	cur_pattern = tetromino_patterns + this_tetromino->type;
	extra_move = empty_right(cur_pattern, this_tetromino->position);

	if (check_collision_right(this_tetromino, this_matrix, cur_pattern, extra_move))
	{
		return false;
	}

	if (this_tetromino->location.left > MATRIX_WIDTH - cur_pattern->width + extra_move)
	{
		this_tetromino->location.left = MATRIX_WIDTH - cur_pattern->width + extra_move;
	}

	return (old_value != this_tetromino->location.left);
}

bool nudge_left(tetromino *this_tetromino, matrix *this_matrix)
{
	const tetromino_pattern *cur_pattern;
	int old_value;
	int extra_move;

	if (this_tetromino->location.left < 0)
	{
		return false;
	}

	old_value = this_tetromino->location.left;
	this_tetromino->location.left--;

	cur_pattern = tetromino_patterns + this_tetromino->type;
	extra_move = empty_left(cur_pattern, this_tetromino->position);

	if (check_collision_left(this_tetromino, this_matrix, cur_pattern, extra_move))
	{
		return false;
	}

	if (this_tetromino->location.left < 0 - extra_move)
	{
		this_tetromino->location.left = 0 - extra_move;
	}

	return (old_value != this_tetromino->location.left);
}

bool nudge_down(tetromino *this_tetromino, matrix *this_matrix)
{
	const tetromino_pattern *cur_pattern;
	int old_value;
	int extra_move;

	if (this_tetromino->location.top < 0)
	{
		return false;
	}

	old_value = this_tetromino->location.top;

	cur_pattern = tetromino_patterns + this_tetromino->type;
	extra_move = empty_bottom(cur_pattern, this_tetromino->position);

	if (check_collision_down(this_tetromino, this_matrix, cur_pattern, extra_move))
	{
		return false;
	}

	this_tetromino->location.top++;

	if (this_tetromino->location.top > MATRIX_DEPTH - cur_pattern->height + extra_move)
	{
		this_tetromino->location.top = MATRIX_DEPTH - cur_pattern->height + extra_move;
	}

	return (old_value != this_tetromino->location.top);
}

int empty_right(const tetromino_pattern *this_pattern, int position)
{
	int width = this_pattern->width;
	int height = this_pattern->height;
	bool stop = false;

	int col;
	for (col = width - 1; col >= 0; col--)
	{
		for (int row = 0; row < height; row++)
		{
			if (this_pattern->pattern[position][height * row + col] != empty)
			{
				stop = true;
				break;
			}
		}

		if (stop)
		{
			break;
		}
	}

	return width - 1 - col;
}

int empty_left(const tetromino_pattern *this_pattern, int position)
{
	int width = this_pattern->width;
	int height = this_pattern->height;
	bool stop = false;

	int col;
	for (col = 0; col < width; col++)
	{
		for (int row = 0; row < height; row++)
		{
			if (this_pattern->pattern[position][height * row + col] != empty)
			{
				stop = true;
				break;
			}
		}

		if (stop)
		{
			break;
		}
	}

	return col;
}

int empty_top(const tetromino_pattern *this_pattern, int position)
{
	int width = this_pattern->width;
	int height = this_pattern->height;
	bool stop = false;

	int row;
	for (row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if (this_pattern->pattern[position][height * row + col] != empty)
			{
				stop = true;
				break;
			}
		}

		if (stop)
		{
			break;
		}
	}

	return row;
}

int empty_bottom(const tetromino_pattern *this_pattern, int position)
{
	int width = this_pattern->width;
	int height = this_pattern->height;
	bool stop = false;

	int row;
	for (row = height - 1; row >= 0; row--)
	{
		for (int col = 0; col < width; col++)
		{
			if (this_pattern->pattern[position][height * row + col] != empty)
			{
				stop = true;
				break;
			}
		}

		if (stop)
		{
			break;
		}
	}

	return height - 1 - row;
}

void insert_tetromino(matrix *this_matrix, tetromino *new_tetromino)
{
	int tetromino_height;
	int tetromino_width;
	const tetromino_pattern *cur_pattern;
	char *pattern;
	int x, y;

	if (new_tetromino->type == illegal_tetromino
		|| new_tetromino->position < 0)
	{
		return;
	}

	cur_pattern = tetromino_patterns + new_tetromino->type;
	tetromino_height = cur_pattern->height;
	tetromino_width = cur_pattern->width;
	pattern = cur_pattern->pattern[new_tetromino->position];

	for (int row = 0; row < tetromino_height; row++)
	{
		for (int col = 0; col < tetromino_width; col++, pattern++)
		{
			if (*pattern != empty)
			{
				y = row + new_tetromino->location.top;
				x = col + new_tetromino->location.left;
				this_matrix->squares[MATRIX_WIDTH * y + x] = toupper(*pattern);
			}
		}
	}
}

void print_all(game_state *this_game_state)
{
	matrix temp_matrix;
	strcpy_s(temp_matrix.squares, MATRIX_WIDTH * MATRIX_DEPTH + 1,
		this_game_state->main_matrix.squares);
	insert_tetromino(&temp_matrix, &(this_game_state->active_tetromino));
	print_matrix(&temp_matrix);
}

void drop_tetromino(game_state *this_game_state)
{
	matrix *main_matrix;
	tetromino *active_tetromino;

	main_matrix = &(this_game_state->main_matrix);
	active_tetromino = &(this_game_state->active_tetromino);
	
	while (nudge_down(active_tetromino, main_matrix))
		;

	insert_tetromino(main_matrix, active_tetromino);
	for (int row = 0; row < MATRIX_DEPTH; row++)
	{
		for (int col = 0; col < MATRIX_WIDTH; col++)
		{
			main_matrix->squares[MATRIX_WIDTH * row + col] = 
				tolower(main_matrix->squares[MATRIX_WIDTH * row + col]);
		}
	}

	active_tetromino->type = illegal_tetromino;
}
