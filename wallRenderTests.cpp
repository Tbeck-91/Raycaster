#include <stdio.h>
#include "SDLMain.h"
#include "Texture.h"
#include "Raycast.h"

#define ANGLE_INC 5 // player view angle increment/decrement value
#define PLAYER_INC 5 // player movement inc
#define DEBUG_PRINT_1 // only print messages upon error
#define FONT "C:\\Users\\UberTay\\OneDrive\\VS_LIBS\\FONTS\\Xerox Sans Serif Wide.ttf"
//#define NDEBUG

#pragma warning(disable: 4996)

int main(int argc, char** argv) {

	SDLMain testWin("Raycast-Test", 320, 200);
	//Event handler 
	SDL_Event e;
	char dataBuf[256];

	int mapArray[6][8] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	};

	POINT playerCoord = { 96, 224 };
	int playerAngle = 120; // player view angle
	double rayAngleInc = ANGLE_INCREMENT; // angle increment to next ray

	SDL_Rect topHalf = { 0, 0, VIEW_WIDTH >> 1, VIEW_HEIGHT >> 1 };
	SDL_Rect bottomHalf = { 0, VIEW_HEIGHT >> 1, VIEW_WIDTH, VIEW_HEIGHT >> 1 };

	bool quit = false;

	/* Setup Text */

	// TODO: finish this up

	//Globally used font
	TTF_Font *gFont = TTF_OpenFont(FONT, 14);
	if (gFont == NULL) printf("Font failed to load! \n");
	//Render text 
	SDL_Color textColour = { 0, 0, 0 };

	Texture textTexture(testWin.renderer);
	int textHeight;
	int textWidth;
	try {
		// MakeTextTexture(TTF_Font *gFont, char* textureText, SDL_Color textColor, int* textWidth, int* textHeight)
		if (!textTexture.MakeTextTexture(gFont, "Hello World", textColour, &textWidth, &textHeight)) printf("Text Texture could not be created! \n");
	}
	catch (const char* e) {
		printf("Error!: %s \n", e);
		exit(-1);
	}

	SDL_Rect fontRect = { VIEW_WIDTH - (textWidth + 5), VIEW_HEIGHT - (textHeight + 10), textWidth, textHeight };
	int timer = 0, timerLimit = 1000;


	while (!quit) {
		//Handle events on queue 
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit 
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			else if (e.type == SDL_KEYDOWN) {

				switch (e.key.keysym.sym) {

				case SDLK_UP:
					playerCoord.y -= PLAYER_INC;
					if (playerCoord.y < 0) playerCoord.y = 0;
					break;

				case SDLK_DOWN:
					playerCoord.y += PLAYER_INC;
					if (playerCoord.y >= 5 << SHIFT_VALUE) playerCoord.y = (6 << SHIFT_VALUE) - PLAYER_INC;
					break;

				case SDLK_LEFT:
					playerAngle -= ANGLE_INC;
					if (playerAngle > 360) playerAngle -= 360;
					if (playerAngle < 0) playerAngle += 360;
					break;

				case SDLK_RIGHT:
					playerAngle += ANGLE_INC;
					if (playerAngle > 360) playerAngle -= 360;
					break;
				case SDLK_a:
					playerCoord.x -= PLAYER_INC;
					break;

				case SDLK_d:
					playerCoord.x += PLAYER_INC;
					break;

				default:

					break;

				}	// end switch	

			} // end if

		} // end event while

		  // clear screen
		SDL_SetRenderDrawColor(testWin.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		testWin.ClearRenderer();

		// draw floor (gray)
		SDL_SetRenderDrawColor(testWin.renderer, 0x80, 0x80, 0x80, 0xFF);
		SDL_RenderFillRect(testWin.renderer, &bottomHalf);

		// draw walls
		double rayAngle = playerAngle - (FOV / 2);
		int curCol = 0;
		if (rayAngle < 0) rayAngle += 360;

		//SDL_SetRenderDrawColor(testWin.renderer, 0x00, 0x00, 0xFF, 0xFF); 
		/*
		cast a ray for every column of the view dimension,
		and draw a wall slice at the intersection point
		*/

		while (curCol < VIEW_WIDTH) {
			// POINT* playerCoord, double angle, int mapArray[][4])
			// TODO: pass wallIntersection by reference, less overhead
			POINT wallIntersection = castRay(&playerCoord, rayAngle, mapArray);
			int wallDist = getDistanceToWall(&wallIntersection, &playerCoord);
			int z = wallDist * abs(cos(degToRad(rayAngle)));
			int wallHeight = (double)TILE_HEIGHT * (double)wallDist / (double)277;

			wallHeight = wallHeight / z;

			//assert(wallIntersection.x < VIEW_WIDTH && wallIntersection.y < VIEW_HEIGHT);

			//int wallHeight = (double)TILE_HEIGHT * (277 / (double)wallDist);
			int topOfWall = (VIEW_HEIGHT / 2) - (wallHeight / 2);
			int bottomOfWall = topOfWall + wallHeight;
			if (bottomOfWall > VIEW_HEIGHT) bottomOfWall = VIEW_HEIGHT - 1;

			//if (wallIntersection.x >> SHIFT_VALUE == 0) SDL_SetRenderDrawColor(testWin.renderer, 0xFF, 0x00, 0x00, 0xFF); // draw left walls red
			//else if (wallIntersection.x >> SHIFT_VALUE == 8) SDL_SetRenderDrawColor(testWin.renderer, 0x00, 0xFF, 0x00, 0xFF); // draw right walls green
			SDL_SetRenderDrawColor(testWin.renderer, 0x00, 0x00, 0xFF, 0xFF);

			SDL_Rect wallSlice;
			wallSlice.x = curCol;
			wallSlice.y = topOfWall;
			wallSlice.w = 5;
			wallSlice.h = wallHeight;

			SDL_RenderFillRect(testWin.renderer, &wallSlice);

			//SDL_RenderDrawLine(testWin.renderer, curCol, topOfWall, curCol, topOfWall + wallHeight);
			//rayAngle += rayAngleInc;
			rayAngle += 5;
			if (rayAngle >= 360) rayAngle -= 360;
			//curCol++;
			curCol += 5;
		}
		// TODO: make an SDL_Font class wrapper 
		sprintf(dataBuf, "current player angle: %d", playerAngle);
		textTexture.MakeTextTexture(gFont, dataBuf, textColour, &textWidth, &textHeight);
		fontRect.x = VIEW_WIDTH - (textWidth + 5);
		fontRect.y = VIEW_HEIGHT - (textHeight + 10);
		fontRect.w = textWidth;
		fontRect.h = textHeight;
		memset(dataBuf, 0, 256);
		textTexture.RenderText(&fontRect);

		sprintf(dataBuf, "player x: %d, player y: %d", playerCoord.x, playerCoord.y);
		textTexture.MakeTextTexture(gFont, dataBuf, textColour, &textWidth, &textHeight);
		fontRect.x = 5;
		fontRect.y = 5;
		fontRect.w = textWidth;
		fontRect.h = textHeight;
		memset(dataBuf, 0, 256);
		textTexture.RenderText(&fontRect);


		// update renderer
		testWin.UpdateTexture();

	} // end main while

	TTF_CloseFont(gFont);
	gFont = NULL;

	return 0;
}
