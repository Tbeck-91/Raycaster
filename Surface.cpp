#include "Surface.h"

void Surf::blit(void) {
	SDL_BlitSurface(img, NULL, scrn, NULL);
}

// TODO: finish up loading optimized image
// tutorial page : http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index2.php
bool Surf::loadSurf(char* path, SDL_Surface* winScrn) {

	bool success = true;

	// image needs to be formatted to window, if there's no window pointer return error 
	if (scrn == NULL) {
		if (winScrn != NULL) scrn = winScrn;			
		else success = false;
	}

	if (success) {

		SDL_Surface* tmpSurf = IMG_Load(path);

		/*
		   Check if we're already pointing to an image and handle accordingly
		*/
		if (img != NULL) {
			SDL_FreeSurface(img); // free previous image
			img = NULL;
		}	

		if (tmpSurf != NULL) {

			//Convert surface to screen format 
			img = SDL_ConvertSurface(tmpSurf, scrn->format, NULL);

			//Get rid of old loaded surface 
			SDL_FreeSurface(tmpSurf);
		}
	}

	return success;
}

Surf::Surf(char* imgPath, SDL_Surface* winScrn) : scrn(NULL), img(NULL) { 
	 
	 if ( !loadSurf(imgPath, winScrn) )  throw "image could not be loaded!";	
}


