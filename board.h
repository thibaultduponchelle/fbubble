#include "bubble.h"

typedef struct _cell {
  bubble* bubble; 
} cell;

typedef struct _board {
  char* title;
  cell** cells;
  int rows;
  int cols;
} board;

/* Create a board with given fixed size */
board* board_create(int rows, int cols);

/* Create a board with given fixed size */
board* board_create_with_title(int rows, int cols, char* title);

/* Fill board with blank color (null color) */
void board_fill_blank(board* bd);

/* Fill board with random color */
void board_fill_random(board* bd);

/* Print board into console */
void board_print_cli(board* bd);

/* Free the board */
void board_free(board* bd);

/* Get neighbour of a cell */
bubble* board_get_neighbour(board* bd, int x, int y, char direction);

/* Click on a cell */
void board_click(board* bd, int x, int y);

/* Check if a cell is clickable (at least one neighbour with the same color */
bool board_cell_is_clickable(board* bd, int x, int y);

/* Apply gravity ! */
void board_fall_do(board* bd);

/* Check if board is dead */
bool board_is_dead(board* bd);
