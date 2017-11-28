#ifndef __GRAPHICS__
#define __GRAPHICS__

#ifdef __linux__
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#elif __APPLE__
	#include <SDL2/SDL.h>
	#include <SDL2_image/SDL_image.h>
#endif

#define SOURCE_TILE_WIDTH 128
#define SOURCE_TILE_HEIGHT 128

#define DESTINATION_TILE_WIDTH 48
#define DESTINATION_TILE_HEIGHT 48

class Agent;

class Graphics {
  public:
    Graphics();
    ~Graphics();
    void Render(Agent *p_agent);
    void RenderTile(unsigned char p_tile, unsigned int p_dir, int p_x, int p_y);
    int HandleInput(void);
    SDL_Rect SetSource(unsigned char p_tile);

  private:
    int screen_width, screen_height;
    unsigned long delay;
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *sprites;
};

#endif /* __GRAPHICS__ */
