#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100
#define MAX_STACK_SIZE 10000

typedef struct {
	int row;
	int col;
	int dir;
} Stack;

typedef struct {
	int x;
	int y;
} offsets;

Stack stack[MAX_STACK_SIZE];
offsets move[8] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
					{1, 0}, {1, -1}, {0, -1}, {-1, -1} };

int top = -1;

int IsEmpty(Stack *);
int IsFull(Stack *);
void push(Stack);
Stack pop();

void make_path(char **, int, int);
void change_path(char **, int, int);
void path(char **, int **, int);
void print_path(char **, int);

int main(void) {
	char **maze;
	int **mark;
	int n, seed, i, j;

	printf("미로의 크기와 seed값 입력: ");
	scanf("%d %d", &n, &seed);
	if (n > MAX_SIZE) {
		printf("최대 길이가 100입니다.");
		exit(1);
	}

	maze = (char**)malloc(sizeof(char*)*n);
	for (i = 0; i < n; i++)
		maze[i] = (char*)malloc(sizeof(char)*n);

	mark = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++)
		mark[i] = (int*)malloc(sizeof(int)*n);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			*(*(mark + i) + j) = 0;
	}

	make_path(maze, n, seed);
	change_path(maze, n, seed);
	path(maze, mark, n);

	free(maze);
	return 0;
}

int IsEmpty(Stack *stack) {
	if (top == -1)
		return true;
	else
		return false;
}

int IsFull(Stack *) {
	if (top == MAX_STACK_SIZE - 1)
		return true;
	else return false;
}
void push(Stack position) {
	if (IsFull(stack)) {
		printf("스택이 꽉 찼습니다.\n");
		exit(1);
	}
	else
		stack[++top] = position;
}

Stack pop() {
	if (IsEmpty(stack)) {
		printf("스택이 비어있습니다.\n");
		exit(1);
	}
	else
		return stack[top--];
}

void make_path(char **maze, int n, int seed) {
	int i, j, s;
	int x_pos = 0, y_pos = 0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			*(*(maze + i) + j) = '.';
	}

	srand(seed);
	*(*(maze + x_pos) + y_pos) = '0';

	while (1) {
		s = rand() % 100 + 1;

		if (s >= 1 && s <= 30 && (x_pos + 1) < n && (y_pos + 1) < n)
			*(*(maze + (++x_pos)) + (++y_pos)) = '0';
		else if (s >= 31 && s <= 65 && (y_pos + 1) < n)
			*(*(maze + (x_pos)) + (++y_pos)) = '0';
		else if (s >= 66 && s <= 100 && (x_pos + 1) < n)
			*(*(maze + (++x_pos)) + (y_pos)) = '0';

		if (x_pos == n - 1 && y_pos == n - 1)
			break;
	}

	printf("초기 경로: \n");
	print_path(maze, n);
	printf("\n");
}

void change_path(char **maze, int n, int seed) {
	int s;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (*(*(maze + i) + j) == '.') {
				s = rand() % 100 + 1;

				if (s >= 1 && s <= 60)
					*(*(maze + i) + j) = '1';
				else if (s >= 61 && s <= 100)
					*(*(maze + i) + j) = '0';
			}
		}
	}

	printf("변경된 미로: \n");
	print_path(maze, n);
	printf("\n");
}

void path(char **maze, int **mark, int n) {
	int i, j, row, col, dir, next_row, next_col;
	int found = false;
	Stack position;

	mark[0][0] = 1; top = 0;
	stack[0].row = 0; stack[0].col = 0; stack[0].dir = 2;

	while (top > -1 && !found) {
		position = pop();
		row = position.row; col = position.col; dir = position.dir;

		while (dir < 8 && !found) {
			next_row = row + move[dir].x; next_col = col + move[dir].y;
			if (next_row < 0 || next_row >= n || next_col < 0 || next_col >= n) {
				dir++;
				continue;
			}

			if (next_row == n - 1 && next_col == n - 1)
				found = true;
			else if (maze[next_row][next_col] == '0' && !mark[next_row][next_col]) {
				mark[next_row][next_col] = 1;
				position.row = row; position.col = col;
				position.dir = ++dir;
				push(position);
				row = next_row; col = next_col;
				dir = 0;
			}
			else
				dir++;
		}
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			*(*(maze + i) + j) = '.';
	}

	for (i = 0; i <= top; i++) {
		maze[stack[i].row][stack[i].col] = '0';
		maze[row][col] = '0';
		maze[n - 1][n - 1] = '0';
	}
	print_path(maze, n);
}

void print_path(char **maze, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			printf("%c ", *(*(maze + i) + j));
		printf("\n");
	}
}