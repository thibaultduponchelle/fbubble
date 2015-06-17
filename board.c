#include "board.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*board* board_create() {
  board* bd = (board*) malloc(sizeof(board));
  return bd;
}*/

board* board_create(int rows, int cols) {
  board* bd = (board*) malloc(sizeof(board));
  bd->rows = rows;
  bd->cols = cols;
  bd->cells = (cell**) malloc(sizeof(cell*) * rows);
  bd->title = NULL;
  int i = 0;
  for(i = 0; i < rows; i++)  {
    bd->cells[i] = (cell*)malloc(cols * sizeof(cell));
  }

  int r = 0;
  int c = 0;
  for(r = 0; r < rows; r++) {
    for(c = 0; c < cols; c++) {
      bd->cells[r][c].bubble = NULL;
    }
  }

  return bd;
}

board* board_create_with_title(int rows, int cols, char* title) {
  board* bd = (board*) malloc(sizeof(board));
  bd->rows = rows;
  bd->cols = cols;
  bd->cells = (cell**) malloc(sizeof(cell*) * rows);
  bd->title = (char*) malloc(sizeof(char) * strlen(title));
  strcpy(bd->title, title);
  bd->title[strlen(title) - 1] = '\0';
  int i = 0;
  for(i = 0; i < rows; i++)  {
    bd->cells[i] = (cell*)malloc(cols * sizeof(cell));
  }

  int r = 0;
  int c = 0;
  for(r = 0; r < rows; r++) {
    for(c = 0; c < cols; c++) {
      bd->cells[r][c].bubble = NULL;
    }
  }

  return bd;
}

void board_fill_blank(board* bd) {
  int i = 0;
  int j = 0;
  for(i = 0; i < bd->rows; i++) 
    for(j = 0; j < bd->cols; j++) {
      if(bd->cells[i][j].bubble) {
        free(bd->cells[i][j].bubble);
        bd->cells[i][j].bubble = NULL;
      }
    }
}

void board_fill_random(board* bd) {
  int i = 0;
  int j = 0;
  for(i = 0; i < bd->rows; i++) 
    for(j = 0; j < bd->cols; j++) 
      bd->cells[i][j].bubble = bubble_set_random_color(bd->cells[i][j].bubble);
}


board* board_load_from_file(char* filename) {
  int line = 0;
  char c;
  FILE *f;
  char str[100];
  char title[100];
  board* bd = NULL;
  int rows = 0;
  int cols = 0; 
  int i, j;
  if(f = fopen(filename, "r")) {
    while((c = fgetc(f)) != EOF) { 
      if(c == '\n') {
        printf("Newline\n");
        line ++;
      } else {
      	if(line == 0) {
	  fseek(f , -1 , SEEK_CUR);
	  fgets(title, 100, f);
	  fseek(f , -1 , SEEK_CUR);
          printf("Reading the data line %s, %d\n", title, strlen(title));
	} else if(line == 1) {
	  //ungetc(c, f);
	  fseek(f , -1 , SEEK_CUR);
	  fgets(str, 100, f);
	  fseek(f , -1 , SEEK_CUR);
          printf("Reading the rows : %d\n", atoi(str) );
	  rows = atoi(str);
	} else if(line == 2) {
	  fseek(f , -1 , SEEK_CUR);
	  fgets(str, 100, f);
	  fseek(f , -1 , SEEK_CUR);
          printf("Reading the cols : %d\n", atoi(str));
	  cols = atoi(str);
	} else {
	  if(line == 3) {
	    bd = board_create_with_title(rows, cols, title);
	    i = 0;
	    board_fill_blank(bd);
	  } else {
	    i++;
	  }
	  fseek(f , -1 , SEEK_CUR);
	  fgets(str, 100, f);
	  fseek(f , -1 , SEEK_CUR);
          printf("Reading the data line %s, %d\n", str, strlen(str));
	  if(i <= bd->cols) 
            for(j = 0; j < bd->cols; j++) 
              bd->cells[i][j].bubble = bubble_set_color(bd->cells[i][j].bubble, str[j]);
	}
       
      }
    }
  fclose(f);
  }
  return bd;
}
  

void board_print_cli(board* bd) {
  if(!bd) 
    return;

  int r = 0;
  int c = 0;

  for(r = 0; r < bd->rows; r++) {
    for(c = 0; c < bd->cols; c++) { 
      bubble_print_cli(bd->cells[r][c].bubble);
    }
    printf("\n");
  }
}

void board_print_cli_color(board* bd) {
  if(!bd) 
    return;

  int r = 0;
  int c = 0;

  for(r = 0; r < bd->rows; r++) {
    for(c = 0; c < bd->cols; c++) { 
      bubble_print_cli_color(bd->cells[r][c].bubble);
    }
    printf("\n");
  }
}

bubble* board_get_bubble(board* bd, int x, int y) {
  /*if(x < 0 || x >= bd->cols){
    return NULL;
  }
  if (y < 0 || y >= bd->rows) {
    return NULL;
  }*/
  if(board_are_coords_out_of_bounds(bd, x, y)) 
    return NULL;
  return bd->cells[y][x].bubble; // Warning : Could be NULL !
}


bool board_are_coords_out_of_bounds(board* bd, int x, int y) {
   if(x < 0 || x >= bd->cols){
    return true;
  }
  if (y < 0 || y >= bd->rows) {
    return true;
  }
  return false;
}

bool board_is_dead(board* bd) {
  bool result = true;
  int c, r;
  for(r = 0; r < bd->rows; r+=1) {
    for(c = 0; c < bd->cols; c+=1) { 
	if(board_cell_is_clickable(bd, c, r)) {
		result = false;
		// don't break until I put a pause inside event loop
	}
    }
  }

  return result;

}

bool board_is_blank(board* bd) {
  bool result = true;
  int c, r;
  for(r = 0; r < bd->rows; r++) {
    for(c = 0; c < bd->cols; c++) { 
	//if(board_cell_is_clickable(bd, c, r)) {
        bubble* b = board_get_bubble(bd, c, r);
	if(b)
		result = false; // Fails if one or more bubble are on the board
		// don't break until I put a pause inside event loop
    }
  }

  return result;

}
  

bool board_cell_is_clickable(board* bd, int x, int y) {
  bubble* b = board_get_bubble(bd, x, y);
  // Test if there's something here
  if(!b) {
    //printf("There's no bubble here (%d, %d) \n", x, y);
    return false;
  }
  bubble *n, *s, *e, *w;

  n = board_get_neighbour(bd, x, y, 'N');
  s = board_get_neighbour(bd, x, y, 'S');
  e = board_get_neighbour(bd, x, y, 'E');
  w = board_get_neighbour(bd, x, y, 'W');

  if(n) 
    if(n->color == b->color)
      return true;
  if(s) 
    if(s->color == b->color)
      return true;
  if(e) 
    if(e->color == b->color)
      return true;
  if(w) 
    if(w->color == b->color)
      return true;

  return false;
}



void board_click(board* bd, int x, int y) {
  // Test if there's something here
  if(!board_cell_is_clickable(bd, x, y)) {
    printf("Click on %d,%d does anything ! (out of bound or no neighbours)\n", x, y);
    return;
  }
  bubble* b = board_get_bubble(bd, x, y);
  if(bubble_is_exploding(b)) // Protect from looping. Without this protection, stack overflow !
    return;

  bubble_explode_start(b);
  bubble *n, *s, *e, *w;
  n = board_get_neighbour(bd, x, y, 'N');
  s = board_get_neighbour(bd, x, y, 'S');
  e = board_get_neighbour(bd, x, y, 'E');
  w = board_get_neighbour(bd, x, y, 'W');

  if(n) 
    if(n->color == b->color) {
      printf("N");
      board_click(bd, x, y-1);
    }
  if(s) 
    if(s->color == b->color) {
      printf("S");
      board_click(bd, x, y+1);
    }
  if(e) 
    if(e->color == b->color) {
      printf("E");
      board_click(bd, x+1, y);
    }
  
  if(w) 
    if(w->color == b->color) {
      printf("W");
      board_click(bd, x-1, y);
    }

  return;
}

bubble* board_get_neighbour(board* bd, int x, int y, char direction) {
  if(direction == 'N') {
    return board_get_bubble(bd, x, y - 1);
  } else if(direction == 'S') {
    return board_get_bubble(bd, x, y + 1);
  } else if(direction == 'E') {
    return board_get_bubble(bd, x + 1, y);
  } else if(direction == 'W') {
    return board_get_bubble(bd, x - 1, y);
  }
  return NULL;
}

void board_explode_do(board* bd) {
  if(!bd) 
    return;

  int r = 0;
  int c = 0;

  for(r = 0; r < bd->rows; r++) {
    for(c = 0; c < bd->cols; c++) { 
      bubble_explode_do(&bd->cells[r][c].bubble);
    }
  }
}

void board_fall_do(board* bd) {
  if(!bd) 
    return;

  int r = 0; 
  int c = 0;

  for(r = bd->rows - 2; r >= 0; r--) { // rows - 1 : because you can't fall from last row
    for(c = 0; c < bd->cols; c++) { 
      if(bd->cells[r][c].bubble) {
        bubble* s = board_get_neighbour(bd, c, r, 'S'); 

        if(!s) {
          // Nothing below, so let's fall
          if(!bubble_is_falling(bd->cells[r][c].bubble))
            bubble_fall_start(bd->cells[r][c].bubble);  // Start falling
          else
            bubble_fall_do(bd->cells[r][c].bubble);     // Already falling, simply go on

          if(bd->cells[r][c].bubble->state->yoffset >= 3) { // Aligned sprite? Change cell !
            //printf("Free !\n");
            bd->cells[r+1][c].bubble = bubble_new();
            memcpy(bd->cells[r+1][c].bubble, bd->cells[r][c].bubble, sizeof(bubble));
            bd->cells[r+1][c].bubble->state->yoffset = 0;
            free(bd->cells[r][c].bubble);
            bd->cells[r][c].bubble = NULL;

          }
        } else {
          if(bubble_is_falling(bd->cells[r][c].bubble))
            bubble_fall_end(bd->cells[r][c].bubble);
        }
      }
    }
  }

}

void board_free(board* bd) {
  if(bd)
    free(bd);
}


#ifdef TESTBOARD
int main(int argc, char* argv[]) {
  printf("Create board\n");
  board* bd = board_create(10, 10);
  printf("Print blank board\n\n");
  board_print_cli(bd);
  printf("\n\n\n");
  board_fill_random(bd);
  printf("Print random board\n\n");
  board_print_cli(bd);
  printf("\n\n\n");
  printf("Print random board (color)\n\n");
  board_print_cli_color(bd);
  printf("\n\n\n");

  bubble* a = board_get_bubble(bd, 0, 0);
  bubble* b = board_get_bubble(bd, 0, 1);
  bubble* c = board_get_bubble(bd, 0, 2);
  bubble* d = board_get_bubble(bd, 0, 3);
  bubble* e = board_get_bubble(bd, 0, 4);
  bubble* f = board_get_bubble(bd, 0, 5);
  printf("Print abcdef from previous board\n");
  bubble_print_cli_color(a);
  bubble_print_cli_color(b);
  bubble_print_cli_color(c);
  bubble_print_cli_color(d);
  bubble_print_cli_color(e);
  bubble_print_cli_color(f);
  printf("\n\n\n");
  printf("Reset board, set everything to NULL (empty cells)\n");
  board_fill_blank(bd);
  board_print_cli_color(bd);
  printf("\n\n\n");

  printf("(Re) fill randomly now\n");
  board_fill_random(bd);
  board_print_cli_color(bd);
  printf("\n\n\n");

  printf("Try getting values out of bound\n");
  //printf("10, 9\n");
  a = board_get_bubble(bd, 10, 9);
  bubble_print_cli_color(a);
  //printf("\n9, 10\n");
  a = board_get_bubble(bd, 9, 10);
  bubble_print_cli_color(a);
  //printf("\n10, 10\n");
  a = board_get_bubble(bd, 10, 10);
  bubble_print_cli_color(a);
  //printf("\n0, -1\n");
  a = board_get_bubble(bd, 0, -1);
  bubble_print_cli_color(a);
  //printf("\n10, 9\n");
  a = board_get_bubble(bd, -1, 0);
  bubble_print_cli_color(a);
  printf("\n\n\n");

  printf("Get neighbours\n"); 
  a = board_get_neighbour(bd, 0, 0, 'N');
  bubble_print_cli_color(a);
  a = board_get_neighbour(bd, 0, 0, 'S');
  bubble_print_cli_color(a);
  a = board_get_neighbour(bd, 0, 0, 'E');
  bubble_print_cli_color(a);
  a = board_get_neighbour(bd, 0, 0, 'W');
  bubble_print_cli_color(a);
  printf("\n\n\n");

  printf("Now click and just see if bubble could explode or not\n");
  printf("Could explode %d, %d ? %s\n", 0, 0, board_cell_is_clickable(bd, 0, 0) ? "true" : "false");
  printf("Could explode %d, %d ? %s\n", 1, 0, board_cell_is_clickable(bd, 1, 0) ? "true" : "false");
  printf("Could explode %d, %d ? %s\n", 2, 0, board_cell_is_clickable(bd, 2, 0) ? "true" : "false");
  printf("Could explode %d, %d ? %s\n", 3, 0, board_cell_is_clickable(bd, 3, 0) ? "true" : "false");
  printf("Could explode %d, %d ? %s\n", 30, 30, board_cell_is_clickable(bd, 30, 30) ? "true" : "false");
  printf("\n\n\n");

  printf("Simulate a click on a cell\n");
  board_click(bd, 0, 0);
  board_click(bd, 1, 0);
  board_click(bd, 5, 0);
  board_click(bd, 7, 2);
  board_click(bd, 1, 9);
  board_print_cli_color(bd);
  printf("\n");
  board_explode_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  board_explode_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  board_explode_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  printf("\n\n\n");

  printf("Gravity !!!\n");
  board_fall_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  board_fall_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  board_fall_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  board_fall_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  board_fall_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  board_fall_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  board_fall_do(bd);
  board_print_cli_color(bd);
  printf("\n");
  printf("\n\n\n");

  return 0;
}
#endif





