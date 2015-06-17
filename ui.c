#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>


void put_title(SDL_Surface* screen, int SCREENW, int SCREENH, SDL_Surface* title, TTF_Font* font) {

  SDL_Rect source, destination; 
  // put title
  source.x = 0;
  source.y = 0;
  source.w = 95;
  source.h = 49;
  destination.x = 0;
  destination.y = 0;
  destination.w = 90;
  destination.h = 49;

  SDL_BlitSurface(title, &source, screen, &destination);

  int tc = 0;
  source.x = 95;
  source.w = 1;
  for(tc = 95; tc < SCREENW - 90; tc ++) {
    destination.x = tc;
    SDL_BlitSurface(title, &source, screen, &destination);
  }

  source.x = 95;
  source.y = 0;
  source.w = 95;
  source.h = 49;
  destination.x = SCREENW - 95;
  destination.y = 0;
  destination.w = 95;
  destination.h = 49;
  SDL_BlitSurface(title, &source, screen, &destination);

  // Part of the bitmap that we want to draw
  source.x = 0;
  source.y = 0;
  source.w = 24;
  source.h = 24;

  // Part of the screen we want to draw the sprite to
  destination.x = 0;
  destination.y = 0;
  destination.w = 24;
  destination.h = 24;



  // Write text to surface
 
  SDL_Surface *text;
  SDL_Color text_color = {0, 0, 0}; 
  text = TTF_RenderText_Solid(font, "FBubbles", text_color);
  source.x = 0;
  source.y = 0;
  source.w = SCREENW;
  source.h = 49; 
  destination.x = SCREENW / 2 - strlen("FBubbles") * 8;
  destination.y = 10; 
  destination.w = SCREENW;
  destination.h = 24; 
  SDL_BlitSurface(text, &source, screen, &destination);
}
    

void put_caption(SDL_Surface* screen, int SCREENW, int SCREENH, SDL_Surface* title, TTF_Font* font, char* message) {
  SDL_Rect source, destination; 
  // put caption
  source.x = 0;
  source.y = 143;
  source.w = 30;
  source.h = 49;
  destination.x = 0;
  destination.y = (SCREENH + 49) / 2 - 25;
  destination.w = 30;
  destination.h = 49;

  SDL_BlitSurface(title, &source, screen, &destination);

  int tc = 0;
  source.x = 30;
  source.w = 1;
  for(tc = 30; tc < SCREENW - 30; tc ++) {
     destination.x = tc;
     SDL_BlitSurface(title, &source, screen, &destination);
  }

  source.x = 160;
  source.y = 143;
  source.w = 30;
  source.h = 49;
  destination.x = SCREENW - 30;
  destination.y = (SCREENH + 49) / 2 - 25;
  destination.w = 30;
  destination.h = 49;
  SDL_BlitSurface(title, &source, screen, &destination);


  // Write text to surface
  
  SDL_Surface *text;
  //char *str;
  //sprintf(str,"%d",12);
  SDL_Color text_color = {0, 0, 0};
  text = TTF_RenderText_Solid(font, message, text_color);
  source.x = 0;
  source.y = 0;
  source.w = SCREENW;
  source.h = 49;
  destination.x = SCREENW / 2 - (strlen(message) * 7);
  destination.y = (SCREENH + 49) / 2 - 25 + 10;
  destination.w = SCREENW;
  destination.h = 24;
  SDL_BlitSurface(text, &source, screen, &destination);

}

