#include <stdio.h>
#include <ctype.h>

#define MATRIX_WIDTH 10
#define MATRIX_DEPTH 22

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

char matrix[MATRIX_WIDTH][MATRIX_DEPTH];

int score;
int num_lines;

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
			// display active tetramino
			printf("command not implemented %c\n", command);
			break;
		case 'I':
			// set active tetramino to I;
			printf("command not implemented %c\n", command);
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
	for (int y = 0; y < MATRIX_DEPTH; y++)
	{
		for (int x = 0; x < MATRIX_WIDTH; x++)
		{
			printf("%c ", matrix[x][y]);
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
