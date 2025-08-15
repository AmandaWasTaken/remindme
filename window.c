#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "playsound.h"

#ifdef _WIN32
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <SDL2/SDL_ttf.h>

typedef struct {
	int w;
	int h;
	int pos_x;
	int pos_y;
	int lifetime;
} Window_ctx;

Window_ctx ctx = {
	.w 	  = 800,
	.h 	  = 600,
	.lifetime = 10000, // 10 seconds
};

void die(const char* restrict err){

	fprintf(stderr, "Error: %s\nerrno: %i\nSDL Error: %s\n"
			, err, errno, SDL_GetError());
	exit(1);
}

void font_cleanup(TTF_Font* font, 
		SDL_Texture* texture, SDL_Surface* surface){
	
	TTF_CloseFont(font);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	TTF_Quit();
}

void window_cleanup(SDL_Window* window){
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void poll_events(SDL_Window* window){

	SDL_Event e;
	bool running = true;
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT ||
			   e.type == SDL_MOUSEBUTTONDOWN){
				window_cleanup(window);
				running = false;
			}
		}
	}
}

void init_window(const char* restrict content, const int time){
	
	/* Begin window initialization */
	SDL_Window* window = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		die("Failed to initialize SDL");
	}
		
	//with usleep we can do stuff like 0.5 for half a minute 
	usleep(time * 60 * 1000000ULL);
	play_async();

	// @param title(str)
	// @param x coord(int)
	// @param y coord(int)
	// @param width
	// @param height
	window = SDL_CreateWindow(content, SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED,
				  ctx.w, ctx.h, 0);
	if(window == NULL){
		die("Window failed to initialize");
	}	
	/* End window initialization */

	/* Begin font initialization */
	const char* fontname = "./fonts/LiberationMono-Regular.ttf";
	TTF_Init();
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	TTF_Font* font = TTF_OpenFont(fontname, 25);
	if(!font){
		die("Couldn't load font");
	}
	SDL_Color white = {255, 255, 255, 255}; //RGBA
	SDL_Surface* surface = TTF_RenderText_Solid(font, content, white); 
	if(!surface){
		die("Couldn't create surface");
	}
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(
			renderer, surface);
	SDL_FreeSurface(surface);
	if(!texture){
		die("Couldn't create texture");
	}

	int text_w = 0, text_h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &text_w, &text_h);
	SDL_Rect rect = {50, 50, text_w, text_h};
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_RenderPresent(renderer);
	/* End font initialization */

	poll_events(window);
	font_cleanup(font, texture, surface); 
	window_cleanup(window);
}
