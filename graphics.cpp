#include "graphics.h"
#include "agent.h"

/*
 * Initializes the window and renderer to draw graphics. If these are both correctly
 * initialized, this function loads in 'images.png'.
 */
Graphics::Graphics() {
  delay = 251;

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
  TTF_Init();
}

Graphics::~Graphics() {
  SDL_DestroyTexture(sprites);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

/*
 * Renders the map of the algorithm to the window.
 * @param p_agent - a pointer to the agent whose knowledge we want to draw to the screen.
 * @param p_man_x - the 
 */
void Graphics::Render(Agent *p_agent, int n) {
  unsigned int row, col, dir, x_offset, y_offset;
  unsigned char tile;
  SDL_Rect src, dst;

  SDL_RenderClear(renderer);

  src.w = SOURCE_TILE_WIDTH;
  src.h = SOURCE_TILE_HEIGHT;
  dst.w = DESTINATION_TILE_WIDTH;
  dst.h = DESTINATION_TILE_HEIGHT;

  row = n;
  col = n;

  x_offset = (screen_width-DESTINATION_TILE_WIDTH*col)/2;
  y_offset = (screen_height-DESTINATION_TILE_HEIGHT*row)/2;

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      tile = p_agent->get_internal_bits(i, j, &dir);

      RenderTile(tile, dir, j*DESTINATION_TILE_WIDTH+x_offset, i*DESTINATION_TILE_HEIGHT+y_offset);
    }
  }

  SDL_RenderPresent(renderer);
  HandleInput();
  SDL_Delay(delay);
}

void Graphics::RenderTile(unsigned char p_tile, unsigned int p_dir, int p_x, int p_y) {
  SDL_Rect src, dst;

  src.w = SOURCE_TILE_WIDTH;
  src.h = SOURCE_TILE_HEIGHT;
  src.x = src.y = 0;

  dst.w = DESTINATION_TILE_WIDTH;
  dst.h = DESTINATION_TILE_HEIGHT;
  dst.x = p_x;
  dst.y = p_y;

  if ((p_tile & 0x80) == 0x80) { /* if unknown */
    SDL_RenderFillRect(renderer, &dst);
  } 
  else { /* if known */
    src.x = 3*SOURCE_TILE_WIDTH;
    src.y = 1*SOURCE_TILE_WIDTH;
    SDL_RenderCopy(renderer, sprites, &src, &dst);

    if ((p_tile & 0x10) == 0x10) { /* if gold */
      src.x = 1*SOURCE_TILE_WIDTH;
      src.y = 1*SOURCE_TILE_HEIGHT;
      SDL_RenderCopy(renderer, sprites, &src, &dst);
    }

    if((p_tile & 0x20) == 0x20) { /*if man */
      src.x = 0;
      src.y = 1*SOURCE_TILE_WIDTH;
      SDL_RenderCopy(renderer, sprites, &src, &dst);
    }
    if ((p_tile & 0x8) == 0x8) { /* if wumpus */
      src.x = 1*SOURCE_TILE_WIDTH;
      src.y = 0;
      SDL_RenderCopy(renderer, sprites, &src, &dst);
    }
    if ((p_tile & 0x2) == 0x2) { /* if pit */
      src.x = 3*SOURCE_TILE_WIDTH;
      src.y = 0;
      SDL_RenderCopy(renderer, sprites, &src, &dst);
    }
    if ((p_tile & 0x4) == 0x4) { /* if stench */
      src.x = 0;
      src.y = 0;
      SDL_RenderCopy(renderer, sprites, &src, &dst);
    }
    if ((p_tile & 0x1) == 0x1) { /* if draft */
      src.x = 2*SOURCE_TILE_WIDTH;
      src.y = 0;
      SDL_RenderCopy(renderer, sprites, &src, &dst);
    }
  }

  src.x = 2*SOURCE_TILE_WIDTH;
  src.y = 1*SOURCE_TILE_HEIGHT;

  dst.w /= 4;
  dst.h /= 4;

  dst.y += dst.h*3;

  switch (p_dir) {
    case none:
      break;
    default:
      SDL_RenderCopyEx(renderer, sprites, &src, &dst, 90.0*p_dir, NULL, SDL_FLIP_NONE);
  }
}

int Graphics::HandleInput(void) {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
        {
          exit(0);
        }
        break;
			case SDL_KEYDOWN:
				{
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            exit(0);
					} else if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
            if (delay < 1000) {
              delay += 50;
            } else {
              delay = 1000;
            }
          } else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            if (delay > 50) {
              delay -= 50;
            } else {
              delay = 50;
            }
          } else if (event.key.keysym.scancode == SDL_SCANCODE_R) {
            return 0;
          }
				}
				break;
			default:
				break;
		}
	}

  return 1;
}

SDL_Rect Graphics::SetSource(unsigned char p_tile) {
  SDL_Rect src;

  src.w = SOURCE_TILE_WIDTH;
  src.h = SOURCE_TILE_HEIGHT;

  if ((p_tile & 0x8) == 0x8) {
    src.x = 7*SOURCE_TILE_WIDTH;
    src.y = 3*SOURCE_TILE_HEIGHT;
  } else if ((p_tile & 0x4) == 0x4) {
    src.x = 3*SOURCE_TILE_WIDTH;
    src.y = 0; 
  } else if ((p_tile & 0xF) == 0) {
    src.x = 12*SOURCE_TILE_WIDTH;
    src.y = 3*SOURCE_TILE_HEIGHT;
  }

  return src;
}

void Graphics::TextInit() {
  TTF_Font* Sans = TTF_OpenFont("sans.ttf", 24); //this opens a font style and sets a size

  SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "text render test", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

  Message_rect.x = (screen_width/2)-100;  //controls the rect's x coordinate 
  Message_rect.y = screen_height/2; // controls the rect's y coordinte
  Message_rect.w = 200; // controls the width of the rect
  Message_rect.h = 100; // controls the height of the rect
}

void Graphics::RenderWin() {

//Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance

//Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

  SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
  SDL_RenderPresent(renderer);

//Don't forget too free your surface and texture

}

void Graphics::RenderLose() {

}

