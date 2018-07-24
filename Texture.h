#pragma once
#include "Surface.h"

/*
TEXTURE

DESCRIPTION: Holds an SDL texture created from an SDL image
Must be instanciated with an SDL renderer. Throws
a char* exception during  construction otherwise.
*/
typedef struct Texture : public Surf {

	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int drawColour;

	/* Constructor / Destructor */

	Texture(SDL_Renderer* _renderer) : renderer(_renderer), texture(NULL), Surf() {				
		if (renderer == NULL) throw "Texture must be instanciated with renderer!";
	};
	Texture(char* imgPath, SDL_Surface* winScrn, SDL_Renderer* _renderer) : renderer(_renderer), texture(NULL), Surf(imgPath, winScrn) {
		if (renderer == NULL) throw "Texture must be instanciated with renderer!";
	};
	~Texture() { if (texture != NULL) SDL_DestroyTexture(texture); }

	/* Utility */
	bool makeTexture();
	bool MakeTextTexture(TTF_Font *gFont, char* textureText, SDL_Color textColor, int* textWidth, int* textHeight);
	void RenderTexture();
	void RenderTexture(SDL_Rect* sourceRect, SDL_Rect* destRect);
	void RenderText(SDL_Rect* renderQuad);
	
	
} TEXTURE;