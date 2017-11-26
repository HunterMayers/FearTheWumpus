#ifndef __GRAPHICS__
#define __GRAPHICS__

#ifdef __linux__
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#elif __APPLE__
	#include <SDL2/SDL.h>
	#include <SDL2_image/SDL_image.h>
#endif

#include "tinyRandomMap.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

class Graphics {
  public:
    Graphics();
    ~Graphics();
    void Render(tinyRandomMap *p_map);
    SDL_Rect SetSource(unsigned char p_tile);

  private:
    int screen_width, screen_height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *sprites;
};

#endif /* __GRAPHICS__ */
