#include "Texture.h"

// create texture from image
bool Texture::makeTexture() {

	bool success = true;

	if (img == NULL) success = false;

	else {

		// free previous texture 
		if (texture != NULL) SDL_DestroyTexture(texture);

		texture = SDL_CreateTextureFromSurface(renderer, img);
		if (texture == NULL) success = false;
	}
	return success;
}

// copied from: http://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php
// TODO: will have to change this code to make it non-plagiaristic
bool Texture::MakeTextTexture(TTF_Font *gFont, char* textureText, SDL_Color textColor, int* textWidth, int* textHeight) {

	//Get rid of preexisting texture 
	if (texture != NULL) SDL_DestroyTexture(texture);

	//Render text surface 
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText, textColor ); 
	if( textSurface == NULL ) {
		throw "Unable to render text surface!"; 
	} 
	else {
		//Create texture from surface pixels 
		texture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if(texture == NULL ) {
			throw "Unable to create texture from rendered text! SDL Error: %s";
		} 
		else {
			//Get image dimensions
			*textWidth = textSurface->w; 
			*textHeight = textSurface->h;
		} 

		//Get rid of old surface
		SDL_FreeSurface( textSurface );

	} //Return success 
		
	return texture != NULL;
}

void Texture::RenderTexture() {

	//Render texture to screen 
	SDL_RenderCopy( renderer, texture, NULL, NULL );
}

void Texture::RenderTexture(SDL_Rect * sourceRect, SDL_Rect * destRect) {
	// render portion of source to destination rect. Source will be stretched to fit entire destination
	SDL_RenderCopy(renderer, texture, sourceRect, destRect);
}

void Texture::RenderText(SDL_Rect* renderQuad) {

	// TODO: Add default params
	SDL_RenderCopyEx(renderer, texture, NULL, renderQuad, 0.0, NULL, SDL_FLIP_NONE);	

}

