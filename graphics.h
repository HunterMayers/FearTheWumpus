#ifndef __GRAPHICS__
#define __GRAPHICS__

#ifdef __linux__
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
  #include <SDL2/SDL_ttf.h>
#elif __APPLE__
	#include <SDL2/SDL.h>
	#include <SDL2_image/SDL_image.h>
  #include <SDL2_ttf/SDL_ttf.h>
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
    void Render(Agent *p_agent, int n, bool present = true);
    void RenderTile(unsigned char p_tile, unsigned int p_dir, int p_x, int p_y);
    int HandleInput(void);
    SDL_Rect SetSource(unsigned char p_tile);
    void RenderWin();
    void RenderLose();
    void TextInit();

  private:
    int screen_width, screen_height;
    unsigned long delay;
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *sprites;
    SDL_Texture *WinMessage;
    SDL_Texture *LoseMessage;
    SDL_Texture *InstructionMessage;
    SDL_Rect Message_rect;
};

#endif /* __GRAPHICS__ */
