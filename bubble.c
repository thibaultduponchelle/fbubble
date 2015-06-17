#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include "bubble.h"


char AVAILABLE_COLORS[5] = { 'r', 'g', 'b', 'y', '_'};

bubble* bubble_new() {
  bubble* b = (bubble*) malloc(sizeof(bubble));
  b->state = (state*) malloc(sizeof(state));
  b->color = '_';
  b->state->xoffset = 0;
  b->state->yoffset = 0;
  b->state->exploding = 0;
  b->state->falling = 0;
  return b;
}

void bubble_print_cli(bubble* b) {
  if(!b) {
    printf("[  .  ] ");
    return;
  }
  if(!b->state) {
    printf("[  ;  ]\n");
    return;
  }
  printf("[%d-%d-%c] ", b->state->exploding, b->state->yoffset, b->color);
}

void bubble_print_cli_color(bubble* b) {
  if(b == NULL) {
    printf("[  .  ] ");
    return;
  }
  if(b->state == NULL) {
    printf("[  ;  ] ");
    return;
  }
  
  if(b->color == 'r') {
    printf("\033[31m[%d-%d-r]\033[0m ", b->state->exploding, b->state->yoffset);
  } else if(b->color == 'g') {
    printf("\033[32m[%d-%d-g]\033[0m ", b->state->exploding, b->state->yoffset);
  } else if(b->color == 'y') {
    printf("\033[33m[%d-%d-y]\033[0m ", b->state->exploding, b->state->yoffset);
  } else if(b->color == 'b') {
    printf("\033[34m[%d-%d-b]\033[0m ", b->state->exploding, b->state->yoffset);
  } 

}

bubble* bubble_set_color(bubble* b, char color) {
  bool found = false;
  int i = 0;
  for(i = 0; AVAILABLE_COLORS[i]; i++) { // NULL terminated
    if(color == AVAILABLE_COLORS[i]) {
      found = true;
      //break; // No break to keep timing
    }
  }

  if(found) {
    if(!b) {
      //printf("Bubble was null, create it !\n");
      b = bubble_new();
    }
    b->color = color;
  }
  return b;
}

bubble* bubble_set_random_color(bubble* b) {
  int r = rand() % 4;
  b = bubble_set_color(b, AVAILABLE_COLORS[r]);
  return b;

}

void bubble_explode_start(bubble* b) {
  if(!b)
    return;
  if(!b->state)
    return;
  //b->color = '_';
  b->state->exploding = 255;

  //b->state->exploding = 3;
}

void bubble_explode_do(bubble** b) {
  
  if(!(b))
    return;
  if(!(*b))
    return;
  if(!(*b)->state) 
    return;
  if(!bubble_is_exploding(*b)) {
    //printf("Try to explode a bubble without starting explosion !\n");
    return;
  }
  if ((*b)->state->exploding > 0) {
    (*b)->state->exploding -= 20;
    if((*b)->state->exploding <= 0) {
      free((*b)->state);
      free(*b);
      *b = NULL;
    }
  } 
}

bool bubble_is_exploding(bubble* b) {
  if(!b) 
    return false;
  if(!b->state)
    return false;
  return b->state->exploding > 0;
}

bool bubble_is_falling(bubble* b) {
  if(!b)
    return false;
  if(!b->state)
    return false;
  return b->state->falling;
}

void bubble_fall_start(bubble* b) {
  if(!b)
    return;
  if(!b->state)
    return;

  b->state->falling = true;
  b->state->yoffset = 1;
  b->state->yvelocity = 1;
}

void bubble_fall_do(bubble* b) {
  if(!b)
    return;
  if(!b->state)
    return;
  if(b->state->falling == true) {
    b->state->yoffset += b->state->yvelocity;
  }
}

void bubble_fall_end(bubble* b) {
  if(!b)
    return;
  if(!b->state)
    return;
  b->state->falling = false;
  b->state->yoffset = 0;
}

  

bool bubble_is_moving(bubble* b) {
  if(!b->state)
    printf("b->state is null\n");
  return b->state->moving;
}

void bubble_free(bubble* b) {
  if(b)
    free(b);
}


#ifdef TESTBUBBLE
int main(int argc, char* argv[] ){
  
  bubble *a, *b, *c, *d, *e, *f, *g, *h;

  a = bubble_new();
  b = bubble_new();
  c = bubble_new();
  d = bubble_new();
  e = bubble_new();
  f = bubble_new();
  g = bubble_new();
  h = bubble_new();

  printf("Print default bubbles\n");
  bubble_print_cli(a);
  bubble_print_cli(b);
  bubble_print_cli(c);
  bubble_print_cli(d);
  bubble_print_cli(e);
  bubble_print_cli(f);
  bubble_print_cli(g);
  bubble_print_cli(h);
  printf("\n\n");

  bubble_set_color(a, 'r');
  bubble_set_color(b, 'r');
  bubble_set_color(c, 'g');
  bubble_set_color(d, 'g');
  bubble_set_color(e, 'b');
  bubble_set_color(f, 'b');
  bubble_set_color(g, 'y');
  bubble_set_color(h, 'y');
  printf("Print rrggbbyy bubbles\n");
  bubble_print_cli(a);
  bubble_print_cli(b);
  bubble_print_cli(c);
  bubble_print_cli(d);
  bubble_print_cli(e);
  bubble_print_cli(f);
  bubble_print_cli(g);
  bubble_print_cli(h);
  printf("\n\n");

  bubble_set_random_color(a);
  bubble_set_random_color(b);
  bubble_set_random_color(c);
  bubble_set_random_color(d);
  bubble_set_random_color(e);
  bubble_set_random_color(f);
  bubble_set_random_color(g);
  bubble_set_random_color(h);
  printf("Print random bubbles\n");
  bubble_print_cli(a);
  bubble_print_cli(b);
  bubble_print_cli(c);
  bubble_print_cli(d);
  bubble_print_cli(e);
  bubble_print_cli(f);
  bubble_print_cli(g);
  bubble_print_cli(h);
  printf("\n\n");

  printf("Test if bubble is exploding\n");
  printf("Is exploding ? %s\n", bubble_is_exploding(a)? "yes": "no");
  printf("Is exploding ? %s\n", bubble_is_exploding(b)? "yes": "no");
  bubble_explode_start(a);
  bubble_explode_start(b);
  printf("Is exploding ? %s\n", bubble_is_exploding(a)? "yes": "no");
  printf("Is exploding ? %s\n", bubble_is_exploding(b)? "yes": "no");
  bubble_explode_do(&a);
  bubble_print_cli_color(a);
  bubble_explode_do(&a);
  bubble_print_cli_color(a);
  bubble_explode_do(&a);
  bubble_print_cli_color(a);
  //printf("Current explosion %d\n", a->state->exploding);

  printf("\n\n");

  printf("Print colored bubblers with real colors !\n");
  bubble_print_cli_color(a);
  bubble_print_cli_color(b);
  bubble_print_cli_color(c);
  bubble_print_cli_color(d);
  bubble_print_cli_color(e);
  bubble_print_cli_color(f);
  bubble_print_cli_color(g);
  bubble_print_cli_color(h);
  printf("\n\n");

  printf("Try to use functions with a null bubble !\n");
  printf("ok - ");
  bubble_explode_start(NULL);
  printf("ok - ");
  bubble_explode_do(NULL);
  printf("ok - ");
  bubble_fall_start(NULL);
  printf("ok - ");
  bubble_fall_do(NULL);
  printf("ok - \n");
  bubble_print_cli(NULL);
  printf("\nok - \n");
  bubble_print_cli_color(NULL);
  printf("\n\n\n");


  return 0;
}
#endif
