#include "stdbool.h"
#include "stdio.h"
#include <assert.h>

const int ROWS = 9, COLS = 9;                     // Sizes of the board.
const char CIRCLE = 'o', CROSS = 'x', WALL = '+'; // Board fields.

// Overwrites all the fields of the board with given value.
void fill_with(char board[ROWS][COLS], char value) {
  for (int i = 0; i < ROWS; i++)
    for (int j = 0; j < COLS; j++)
      board[i][j] = value;
}

// Prints a separator for the board.
void print_separator() {
  printf(" ");
  for (int i = 0; i < 2 * ROWS + 1; i++)
    printf("%c", WALL);
  printf("\n");
}

// Prints the game board to the terminal.
void display_board(char board[ROWS][COLS]) {
  // Display row indices at the top of the board.
  printf("  ");
  for (int i = 0; i < ROWS; i++)
    printf("%i ", i + 1);
  printf("\n");

  print_separator();
  for (int i = 0; i < ROWS; i++) {
    printf("%i", i + 1); // Print column indices.

    for (int j = 0; j < COLS; j++)
      printf("%c%c", WALL, board[i][j]);
    printf("%c\n", WALL);

    print_separator();
  }
}

// Switches the value of the given field - from CROSS to CIRCLE or from CROSS
// TO CIRCLE. If the indices are outside of board it is ignored.
void switch_field(char board[ROWS][COLS], int row, int col) {
  if (!(row >= 0 && row < ROWS && col >= 0 && col < COLS))
    return;

  if (board[row][col] == CROSS)
    board[row][col] = CIRCLE;
  else if (board[row][col] == CIRCLE)
    board[row][col] = CROSS;
}

// Tries to play according to the user-given input. Returns true, if the move
// was made (if it was valid), false otherwise.
bool put(char board[ROWS][COLS], int row, int col) {
  // Validate input.
  if (row < 1 || row > ROWS || col < 1 || col > COLS)
    return false;

  // Decrement from input values to get zero-based indices.
  --row;
  --col;

  // Switch the field selected and those adjacent to it.
  switch_field(board, row, col);
  switch_field(board, row - 1, col);
  switch_field(board, row, col - 1);
  switch_field(board, row + 1, col);
  switch_field(board, row, col + 1);

  return true;
}

// Returns whether the board contains only the given value.
bool is_full_of(char board[ROWS][COLS], char value) {
  bool is_filled = true;
  for (int i = 0; i < ROWS; i++)
    for (int j = 0; j < COLS; j++)
      if (board[i][j] != value)
        is_filled = false;
  return is_filled;
}

int main() {
  // Initialize game board and move counter.
  char board[ROWS][COLS];
  fill_with(board, CIRCLE);
  int move_count = 0;

  // Game loop (each iteration is a round).
  while (true) {
    display_board(board);
    printf("Moves made: %i\n", move_count);

    // Accept user input.
    int row, col;
    printf("Input row nr: ");
    scanf("%i", &row);
    printf("Input column nr: ");
    scanf("%i", &col);

    // If the user chose valid move, the move counter is incremented. If not,
    // the user will be prompted to try again.
    if (put(board, row, col)) {
      ++move_count;
      // Checks whether the game should end (if the board is full).
      if (is_full_of(board, CROSS)) {
        printf("\nCongratulations, you have won!\n");
        break;
      }
    } else {
      printf("Invalid input! The move won't be counted.\n"
             "Row must be within [1, %i] and the column within [1, %i].\n",
             ROWS, COLS);
    }
  }

  return 0;
}
