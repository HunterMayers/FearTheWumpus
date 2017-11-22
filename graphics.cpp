#include "graphics.h"

Graphics::Graphics() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0) {

		window = SDL_CreateWindow("Fear The Wumpus", 0, 0, 320, 240, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window) {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
      
			if (SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height) == 0) {
        if (renderer) {
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	        SDL_SetRenderTarget(renderer, NULL);
          SDL_RenderClear(renderer);

          SDL_RenderPresent(renderer);
        }
      }
    }
  }

  if (renderer) {
		IMG_Init(IMG_INIT_PNG);
		SDL_Surface *holder = IMG_Load("images.png");

    sprites = SDL_CreateTextureFromSurface(renderer, holder);
    SDL_FreeSurface(holder);

		IMG_Quit();
  }	
}

Graphics::~Graphics() {
  SDL_DestroyTexture(sprites);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void Graphics::Render(tinyRandomMap *p_map) {
  unsigned int row, col, x_offset, y_offset;
  SDL_Rect src, dst;

  SDL_RenderClear(renderer);

  src.w = dst.w = TILE_WIDTH;
  src.h = dst.h = TILE_HEIGHT;

  p_map->dimensions(&row, &col);

  x_offset = (screen_width-TILE_WIDTH*col)/2;
  y_offset = (screen_height-TILE_HEIGHT*row)/2;

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      src = SetSource(p_map->get(i, j));

      dst.x = j*TILE_WIDTH+x_offset;
      dst.y = i*TILE_HEIGHT+y_offset;

      SDL_RenderCopy(renderer, sprites, &src, &dst);
    }
  }

  SDL_RenderPresent(renderer);
}

SDL_Rect Graphics::SetSource(unsigned char p_tile) {
  SDL_Rect src;

  src.w = TILE_WIDTH;
  src.h = TILE_HEIGHT;

  if ((p_tile & 0xA) == 0xA) {
    src.x = 1*TILE_WIDTH;
    src.y = 0;
  } else if ((p_tile & 0x8) == 0x8) {
    src.x = 7*TILE_WIDTH;
    src.y = 3*TILE_HEIGHT;
  } else if ((p_tile & 0x4) == 0x4) {
    src.x = 3*TILE_WIDTH;
    src.y = 0; 
  } else if ((p_tile & 0xF) == 0) {
    src.x = 12*TILE_WIDTH;
    src.y = 3*TILE_HEIGHT;
  }

  return src;
}
