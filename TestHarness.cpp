#include "SDLMain.h"
#include "Surface.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	
	SDLMain* testWin = NULL;

	try { 		
		testWin = new SDLMain(); 	
	}
	catch (const char* err) {

		printf("SDL Init failed: %s\n", err);
		exit(-1); 
	}
	catch (std::bad_alloc& allocErr) {

		printf("Failed to allocate new SDLMain object: %s\n", allocErr.what());
		exit(-1);
	}		

	SURFACE evilMickey;

	if (!evilMickey.loadSurf("pic.bmp", testWin->scrnSurf)) {
		printf("Image could not be loaded! \n");
		exit(-1);
	}

	//Fill the surface white
	SDL_FillRect(testWin->scrnSurf, NULL, SDL_MapRGB(testWin->scrnSurf->format, 0xFF, 0xFF, 0xFF));

	// blit image
	evilMickey.blit();

	//Update the window
	testWin->Update();

	//Wait two seconds
	SDL_Delay(2000);		
		

	delete testWin;
	return 0;
}