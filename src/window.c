#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "playsound.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
	int w;
	int h;
	int pos_x;
	int pos_y;
} Window_ctx;

Window_ctx ctx = {
	.w = 800,
	.h = 600,
};

void window_position(int res[2]){

	// If your monitor isn't 1920x1080 ur fucked lol
	// apparently SDL2 can center the window for you but 
	// i already made this function so it shall stay
	res[0] = (1920 - ctx.w)/2;
	res[1] = (1080 - ctx.h)/2; 
}

void die(const char* restrict err){

	fprintf(stderr, "Error: %s\nerrno: %i\nSDL Error: %s\n"
			, err, errno, SDL_GetError());
	exit(1);
}

void font_cleanup(TTF_Font* font, 
		SDL_Texture* texture){
	
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
	TTF_Quit();
}

void window_cleanup(SDL_Window* window, SDL_Renderer* renderer){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void poll_events(){

	SDL_Event e;
	bool running = true;
	// TODO if the window dies on its own instead of user closing it
	// there's a malloc_consolidate() error so uh fix that
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT ||
			   e.type == SDL_MOUSEBUTTONDOWN){
				running = false;
			}
		}
	}
	return;
}

void init_window(const char* restrict content, const int time){
	
	int window_pos[2];
	window_position(window_pos);
	ctx.pos_x = window_pos[0];
	ctx.pos_y = window_pos[1];

	/* Begin window initialization */
	SDL_Window* window = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		die("Failed to initialize SDL");
	}
		
	// We use usleep instead of sleep() to support decimal times eg. 10.5 minutes
	usleep(time * 60 * 1000000ULL);
	play_async();

	// @param title(str)
	// @param x coord(int)
	// @param y coord(int)
	// @param width
	// @param height
	window = SDL_CreateWindow(content, ctx.pos_x, ctx.pos_y,
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
	
	// Color for background
	SDL_SetRenderDrawColor(renderer, 2, 22, 54, 255); 
	SDL_RenderClear(renderer);

	TTF_Font* font = TTF_OpenFont(fontname, 25);
	if(!font){
		die("Couldn't load font");
	}

	SDL_Color white = {166 ,115, 255, 255}; // Color for text
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
	font_cleanup(font, texture);
	window_cleanup(window, renderer);
}
