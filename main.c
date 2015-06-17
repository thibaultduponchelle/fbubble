#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "board.h"
char levels[20][6] = {"01.fb", "02.fb", "03.fb", "04.fb", "05.fb", "06.fb", "07.fb", "08.fb", "09.fb", "10.fb", "11.fb", "12.fb", "13.fb", "14.fb", "15.fb", "16.fb", "17.fb", "18.fb", "19.fb", "20.fb", "21.fb"}; 
int level = 0;

int main (int argc, char *argv[])
{

  bool quit = false;
  int screenw, screenh;
  int i;
  SDL_Surface *screen;


  putenv("SDL_VIDEO_WINDOW_POS");
  putenv("SDL_VIDEO_CENTERED=1");


  printf("Initialization of the board.\n");
  board* bd = board_create(10, 10);
  board_print_cli_color(bd);
  board_fill_random(bd);
  board_print_cli_color(bd);

  if(argc > 1) {
    char buf[6];
    snprintf(buf, sizeof buf, "%02d.fb", atoi(argv[1]));
    bd = board_load_from_file(buf);
    level = atoi(argv[1]) - 1;
  } else {
init:
      bd = board_load_from_file(levels[level]);
  }
  
  printf ("Initializing SDL.\n");
  /* Initializes Audio and the CDROM, add SDL_INIT_VIDEO for Video */
  if (SDL_Init(SDL_INIT_VIDEO)< 0)
  {
    printf("Could not initialize SDL:%s\n", SDL_GetError());
    SDL_Quit();
    
    return 1;
  }

  if(TTF_Init() != 0) {
    printf("Could not initialize TTF:%s\n", SDL_GetError());
    return 1;
  }

  TTF_Font *font;
  font = TTF_OpenFont("./fonts/future_thin.ttf", 24);
  if(font == NULL) {
    printf("Erreur de font %s\n", TTF_GetError());
    return 1;
  }

  printf("Video initialized correctly\n");

  atexit( SDL_Quit ); 

  int SCREENW = bd->cols * 24;
  int SCREENH = bd->rows * 24;
  printf("SCREENW : %d, SCREENH : %d\n", SCREENW, SCREENW);
  screen = SDL_SetVideoMode( SCREENW, SCREENH + 49, 16, SDL_HWSURFACE );
  screen = SDL_SetVideoMode( SCREENW, SCREENH + 49, 16, SDL_HWSURFACE );
  SDL_WM_SetCaption( "Bubble Crusher", 0 ); 

  if( screen == NULL )
  {
      printf( "Can't set video mode: %s\n", SDL_GetError( ) );
      return EXIT_FAILURE;
  }   

  Uint32 clearColor;
  SDL_Rect screenRect;
  screenRect.x = screenRect.y = 0;
  screenRect.w = screen->w;
  screenRect.h = screen->h;
  clearColor = SDL_MapRGB(screen->format, 200, 200, 255);

  SDL_FillRect(screen, &screenRect, clearColor);
  SDL_Flip(screen);

  SDL_Surface *title;
  title = (SDL_Surface*) IMG_Load("pixs/blue_sheet.png");
  title = SDL_DisplayFormat(title);
  Uint32 colorkey = SDL_MapRGB(title->format, 0, 0, 0 );
  SDL_SetColorKey(title, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );

  SDL_Surface *decoration;
  decoration = (SDL_Surface*) IMG_Load("pixs/yellow_sheet.png");
  decoration = SDL_DisplayFormat(decoration);
  colorkey = SDL_MapRGB(decoration->format, 0, 0, 0 );
  SDL_SetColorKey(decoration, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );



  // <SubTexture name="blue_button00.png" x="0" y="94" width="190" height="49"/>

  SDL_Surface *background;
  background = (SDL_Surface*) IMG_Load("pixs/background.png");
  background = SDL_DisplayFormat(background);
  SDL_Surface *blue, *red, *green, *yellow;
  blue = (SDL_Surface*) IMG_Load("pixs/blue.png");
  blue = SDL_DisplayFormat(blue);
  colorkey = SDL_MapRGB(blue->format, 0, 0, 0 );
  SDL_SetColorKey(blue, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
  red = (SDL_Surface*) IMG_Load("pixs/red.png");
  red = SDL_DisplayFormat(red);
  SDL_SetColorKey(red, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
  green = (SDL_Surface*) IMG_Load("pixs/green.png");
  green = SDL_DisplayFormat(green);
  SDL_SetColorKey(green, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
  yellow = (SDL_Surface*) IMG_Load("pixs/yellow.png");
  yellow = SDL_DisplayFormat(yellow);
  SDL_SetColorKey(yellow, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
  SDL_Surface *warning;
  warning = (SDL_Surface*) IMG_Load("pixs/carreaux.png");
  warning = SDL_DisplayFormat(warning);
  SDL_Surface *target;
  target = (SDL_Surface*) IMG_Load("pixs/target.png");
  target = SDL_DisplayFormat(target);
  //SDL_SetColorKey(yellow, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
  SDL_Rect source;
  source.x = 0;
  source.y = 0;
  source.w = 24;
  source.h = 24;
  SDL_Rect destination;
  destination.x = 0;
  destination.y = 0;
  destination.w = 24;
  destination.h = 24;


  for(destination.y = 0; destination.y + destination.h <= SCREENH + 49; destination.y += destination.h) {
    for(destination.x = 0; destination.x + destination.w <= SCREENW; destination.x += destination.w) {
      SDL_BlitSurface(warning, &source, screen, &destination);
    }
  }


  int xpos=0,ypos=0;

  // The event to handle user interactions
  SDL_Event event;

  int numFrame = 0;
  bool game_is_starting = true;

  while(quit == false) {
    while( SDL_PollEvent( &event ) ) {
      int x = 0;
      int y = 0;
      if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(game_is_starting) {
	  game_is_starting = false;
	} else {
          x = event.motion.x;
          y = event.motion.y;
          printf("Down (%d, %d)!!!! \n", x, y);
	  if(y > 49)
            board_click(bd, x/24, (y-49)/24);
	}
      }

      // User press key
      if( event.type == SDL_KEYDOWN ) {
        Uint8 *keystates = SDL_GetKeyState( NULL );
        // User press ESCAPE
        if( keystates[ SDLK_ESCAPE ] ) { 
          quit = true;
        }
      }

      // User press X
      if( event.type == SDL_QUIT ) { 
        quit = true; 
      } 
    }

    destination.x = 0;
    destination.y = 0;
    destination.w = 24;
    destination.h = 24;
    
    
    
    //SDL_FillRect(screen, &screenRect, clearColor);

    source.x = 0;
    source.y = 0;
    source.w = 24;
    source.h = 24;
    board_fall_do(bd);
    board_explode_do(bd);
    int x = 0;
    int y = 0;
    int r = 0;
    int c = 0;
    SDL_GetMouseState(&xpos, &ypos);


    /* Draw background */
    for(destination.y = 49; destination.y + destination.h <= SCREENH + 49; destination.y += destination.h) {
      for(destination.x = 0; destination.x + destination.w <= SCREENW; destination.x += destination.w) {
        SDL_BlitSurface(warning, &source, screen, &destination);
      }
    }
    source.x = 0;
    source.y = 0;
    source.w = 24;
    source.h = 24;
    //printf("xpos : %d, ypos : %d\n", xpos/24, ypos/24);
    for(destination.y = 49; destination.y + destination.h <= SCREENH + 49; destination.y += destination.h) {
      for(destination.x = 0; destination.x + destination.w <= SCREENW; destination.x += destination.w) {
        
        bubble* b;
        b = (bubble*) board_get_bubble(bd, c, r);
        if(b) {
          destination.y += b->state->yoffset * 7;
          if(b->color == 'b') {
            if(bubble_is_exploding(b)) {
              SDL_SetAlpha(blue, SDL_SRCALPHA, b->state->exploding);
            } else {
              SDL_SetAlpha(blue, SDL_SRCALPHA, 255);
              if((destination.x/24 == xpos/24) && (destination.y/24 == ypos/24)) {
                if(board_cell_is_clickable(bd, xpos/24, (ypos-49)/24)) {
                  SDL_SetAlpha(blue, SDL_SRCALPHA, 128);
                SDL_BlitSurface(target, &source, screen, &destination);
                }
              }
            }
            SDL_BlitSurface(blue, &source, screen, &destination);
          } else if(b->color == 'r') {
            if(bubble_is_exploding(b)) {
              SDL_SetAlpha(red, SDL_SRCALPHA, b->state->exploding);
            } else {
              SDL_SetAlpha(red, SDL_SRCALPHA, 255);
              if((destination.x/24 == xpos/24) && (destination.y/24 == ypos/24)) {
                if(board_cell_is_clickable(bd, xpos/24, (ypos-49)/24)) {
                  SDL_SetAlpha(red, SDL_SRCALPHA, 128);
                SDL_BlitSurface(target, &source, screen, &destination);
                }
              }
              
            }
            SDL_BlitSurface(red, &source, screen, &destination);
          } else if(b->color == 'g') {
            if(bubble_is_exploding(b)) {
              SDL_SetAlpha(green, SDL_SRCALPHA, b->state->exploding);
            } else {
              SDL_SetAlpha(green, SDL_SRCALPHA, 255);
              if((destination.x/24 == xpos/24) && (destination.y/24 == ypos/24)) {
                if(board_cell_is_clickable(bd, xpos/24, (ypos-49)/24)) {
                  SDL_SetAlpha(green, SDL_SRCALPHA, 128);
                SDL_BlitSurface(target, &source, screen, &destination);
                }
              }
            }
            SDL_BlitSurface(green, &source, screen, &destination);
          } else if(b->color == 'y') {
            if(bubble_is_exploding(b)) {
              SDL_SetAlpha(yellow, SDL_SRCALPHA, b->state->exploding);
            } else {
              SDL_SetAlpha(yellow, SDL_SRCALPHA, 255);
              if((destination.x/24 == xpos/24) && (destination.y/24 == ypos/24)) {
                if(board_cell_is_clickable(bd, xpos/24, (ypos-49)/24)) {
                  SDL_SetAlpha(yellow, SDL_SRCALPHA, 128);
                SDL_BlitSurface(target, &source, screen, &destination);
                }
              }
            }
            //SDL_BlitSurface(target, &source, screen, &destination);
            SDL_BlitSurface(yellow, &source, screen, &destination);
          }
          destination.y -= b->state->yoffset * 7;
        }
        c++;
      }
      c = 0;
      r++;
    }


    if(board_is_blank(bd)) {
      printf("Blank\n");
      level++;
      goto init;
    } else if(board_is_dead(bd)) {
      printf("Over\n");
      //goto replay;
    }

    if(game_is_starting && bd->title) {
      put_caption(screen, SCREENW, SCREENH, title, font, bd->title);
    }

    put_title(screen, SCREENW, SCREENH, title, font);


    SDL_Flip(screen);
    SDL_Delay(20);
  }
  
  //SDL_Delay( 3000 );

  SDL_FreeSurface(blue);
  SDL_FreeSurface(red);
  SDL_FreeSurface(green);
  SDL_FreeSurface(yellow);
  
  SDL_Quit();
  
  return(0);
}

