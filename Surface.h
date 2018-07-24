#pragma once
#include "SDLMain.h"

/*
	data structure : SURFACE struct
	
	DESCRIPTION: Loads .png, .bmp, and .tif images. Uses
				 SDL_image in its implementation. 
*/
typedef struct Surf {
	
	SDL_Surface* scrn; // points to window surface
	SDL_Surface* img;  // reference to image surface
	
	
	Surf() : scrn(NULL), img(NULL) {}; // default ctor - set members to NULL
	Surf(char* imgPath, SDL_Surface* winScrn); // initialize with image
	~Surf() { if (img != NULL) SDL_FreeSurface(img); } // free referenced image 

	void blit();
	bool loadSurf(char* path, SDL_Surface* winScrn); // load new image 
	

} SURFACE;

