#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>

#define Rows 20
#define Cols 20

void handle_sigint(int sig) {
    (void)sig;
    printf("\033[?25h\n");
    exit(0);
}

void print_board(int rows, int cols, const bool array[rows][cols], int generation, int elapsed){
  printf("\033[H");

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      putchar(array[i][j] ? '#' : '.');
    }
    putchar('\n');
  }

  printf("Generation: %d | Elapsed Time: %ds\n", generation, elapsed);

  fflush(stdout);
}

int count_neighbours(int rows, int cols, const bool array[rows][cols], int r, int c) {
  int neighbours = 0;

  for (int delta_rows = -1; delta_rows <= 1; delta_rows++) {
    for (int delta_cols = -1; delta_cols <= 1; delta_cols++) {
      if (delta_rows == 0 && delta_cols == 0) continue;

      int new_delta_rows = (r + delta_rows + rows) % rows;
      int new_delta_cols = (c + delta_cols + cols) % cols;

      if (array[new_delta_rows][new_delta_cols]) {
	neighbours++;
      }
    }
  }

  return neighbours;
}

void next_generation(int rows, int cols, bool array[rows][cols]) {
  bool temp[rows][cols];

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int neighbours = count_neighbours(rows, cols, array, i, j);

      if (array[i][j]) {
	temp[i][j] = (neighbours == 2 || neighbours == 3);
      } else {
	temp[i][j] = (neighbours == 3);
      }
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      array[i][j] = temp[i][j];
    }
  }
}

int main(void) {

  int generation = 0;

  time_t start_time = time(NULL);
  
  printf("\033[2J\033[?25l");

  bool array[Rows][Cols] = {0};

  array[1][2] = true;
  array[2][3] = true;
  array[3][1] = true;
  array[3][2] = true;
  array[3][3] = true;

  while (1) {

    signal(SIGINT, handle_sigint);
    
    int elapsed = (int)difftime(time(NULL), start_time);

    print_board(Rows, Cols, array, generation, elapsed);

    next_generation(Rows, Cols, array);

    generation++;

    usleep(250000);
  }

  printf("\033[?25h");

  return 0;
}
