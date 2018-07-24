#include "SDLMain.h"

/*

	Default Constructor: Used for testing, sets window to defaults
	Throws const char* exception on initialization failure
	This exception might not always produce a meaningful error upon image initialization failure
*/
SDLMain::SDLMain(void) : winName((const char*)"Test Window"), kWidth(DEFAULT_SCRN_W), kHeight(DEFAULT_SCRN_H) {

	if (!initSDL(&win, winName, kWidth, kHeight, SDL_WINDOW_SHOWN)) throw SDL_GetError();
}

/*
	Main Constructor
	Throws const char* exception on initialization failure
	This exception might not always produce a SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED ) error upon image initialization failure
	Flag param optional
*/
SDLMain::SDLMain(const char* name, int width, int height, int flags) : winName(name), kWidth(width), kHeight(height) { 
	
	if (!initSDL(&win, name, width, height, flags)) throw SDL_GetError();
}

// TODO: have this function return a richer error message
bool SDLMain::initSDL(SDL_Window** win, const char* name, int width, int height, int flags) {

	bool success = true;

	// Initialize SDL		
	if ((SDL_Init (SDL_INIT_VIDEO) < 0) || 
		(*win = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags)) == NULL)		
		success = false;

	/* Get Screen Surface and Renderer */
	scrnSurf = SDL_GetWindowSurface(*win);
	renderer = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);
	if (success && (!(scrnSurf != NULL && renderer != NULL))) success = false;

	/* Init Library Subsystems */
	else if (!(IMG_Init(IMG_FLG) & IMG_FLG)) success = false; 
	else if (TTF_Init() == -1) success = false;

	return success;
}

SDLMain::~SDLMain(void) {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	
	// Quit SDL subsystems
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
}

void SDLMain::Update(void) {
	SDL_UpdateWindowSurface (win);
}

void SDLMain::UpdateTexture() {
	// from wiki: update the screen with any rendering performed since the previous call.
	SDL_RenderPresent( renderer );
}

void SDLMain::ClearRenderer() {
	SDL_RenderClear(renderer);
}

