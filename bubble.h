#include "state.h"
#include "const.h"


typedef struct _bubble {
  state* state;
  char color; 
} bubble;

bubble* bubble_new();

void bubble_print_cli(bubble* b);

void bubble_print_cli_color(bubble* b);

bubble* bubble_set_color(bubble* b, char color);

bubble* bubble_set_random_color(bubble* b);

void bubble_explode_start(bubble* b);

void bubble_explode_do(bubble** b);

void bubble_fall_start(bubble* b);

void bubble_fall_do(bubble* b);

void bubble_fall_end(bubble* b);

bool bubble_is_falling(bubble* b);

bool bubble_is_moving(bubble* b);

void bubble_free(bubble* b);
