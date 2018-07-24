#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define DEFAULT_SCRN_W 640
#define DEFAULT_SCRN_H 580

// TODO: write a proper SDLError class so that I can throw proper exceptions. should inherit from Exception


// initialize image library with all image flags
#define IMG_FLG IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF 

// TODO: should SDLMain be renamed SDL_WIN or something similar? SDLMain is a bit general.

class SDLMain {

private:
	
	const char* winName; // Appears on top of window box
	const int kWidth, kHeight; // window dimensions
	
	bool initSDL(SDL_Window** win,const char* name, int width, int height, int flags);
	
public:

	SDL_Window* win;
	SDL_Surface* scrnSurf;
	SDL_Renderer* renderer;
	
	/* Constructors / Destructor */
	SDLMain(void);
	SDLMain(const char* name, int width, int height, int flags = SDL_WINDOW_SHOWN);
	~SDLMain(void);

	/* Utilities */
	void Update(void); // refreshes window surfaces 
	void UpdateTexture(); // refreshes textures
	void ClearRenderer();	
};

