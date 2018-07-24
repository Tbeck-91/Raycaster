#include "Raycast.h"
#include "SDLMain.h"
#include <stdio.h>

/* Visual Debugging and Testing for my Raycast Algorithm*/


/*
	looks like the initial intersection point is being calculated perfectly,
	while the raycast algorithm itself is having issues. It isn't finding the closest
	intersection point, usually hitting the horizontal intersection after passing through
	a vertical one. Will require a lot more testing.

*/

int main(int argc, char** argv) {

	SDLMain testWin("Raycast-Test", 512, 320);
	bool quit = false;
	SDL_Event e;
	SDL_Rect player = { 94 , 222, 5, 5 };
	bool printedAngle = false;
	int playerAngle = 44;

	int mapArray[5][8] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	};

	POINT playerCoord = { 96, 224 };
	printf("MapArray[5][1] %d \n", mapArray[4][1]);	
	int wallDist = 330;
	int wallHeight = (((double)TILE_HEIGHT / (double)wallDist) * (double)277);
	printf("Wall height: %d\n", wallHeight);

	//SDL_RenderFillRect(testWin.renderer, &bottomHalf);
	//POINT rayHit = castRay(&playerCoord, 270, mapArray);
	//int dist = getDistanceToWall(&rayHit, &playerCoord);
	//printf("The Distance to the wall is %d \n", dist);
	//printf("ray hit grid at: (%d, %d), units: (%d, %d)\n", rayHit.x >> 6, rayHit.y >> 6, rayHit.x, rayHit.y);

	while (!quit) {

		// clear screen
		SDL_SetRenderDrawColor(testWin.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		testWin.ClearRenderer();
		
		//Handle events on queue 
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit 
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			else if (e.type == SDL_KEYDOWN) {

				switch (e.key.keysym.sym) {
				case SDLK_LEFT:
					playerAngle -= 1;
					if (playerAngle < 0) playerAngle += 360;
					break;

				case SDLK_RIGHT:
					playerAngle += 1;
					if (playerAngle >= 360) playerAngle -= 360;
					break;
				case SDLK_a:
					playerCoord.x--;
					break;
				case SDLK_d:
					playerCoord.x++;
					break;
				case SDLK_w:
					playerCoord.y--;
					break;
				case SDLK_s:
					playerCoord.y++;
					break;
				default:
					break;
				}	// end switch	
			}
		} // end event while


		SDL_SetRenderDrawColor(testWin.renderer, 0xFF, 0x00, 0x00, 0xFF);
		for (int y = 64; y <= 320; y += 64) {

			SDL_RenderDrawLine(testWin.renderer, 0, y, 512, y);

		}
		for (int x = 64; x < 512; x += 64) {
			SDL_RenderDrawLine(testWin.renderer, x, 0, x, 320);
		}	

		SDL_SetRenderDrawColor(testWin.renderer, 0x00, 0x00, 0xFF, 0xFF);

		player.x = playerCoord.x;
		player.y = playerCoord.y;
		SDL_RenderFillRect(testWin.renderer, &player);
		SDL_SetRenderDrawColor(testWin.renderer, 0x00, 0xFF, 0x00, 0xFF);

		//int curAngle = playerAngle - 30;
		//if (curAngle < 0) curAngle += 360;
		int curAngle = 99;
		//for (curAngle; curAngle < playerAngle + 30; curAngle += 5) {
		//	POINT rayHit = castRay(&playerCoord, curAngle, mapArray);
		//	SDL_RenderDrawLine(testWin.renderer, playerCoord.x, playerCoord.y, rayHit.x, rayHit.y);
		//}
		POINT rayHit = castRay(&playerCoord, curAngle, mapArray);
		SDL_RenderDrawLine(testWin.renderer, playerCoord.x, playerCoord.y, rayHit.x, rayHit.y);
		/*
		if (!printedAngle) {
			printf("ray hit grid at: (%d, %d), units: (%d, %d)\n", rayHit.x >> 6, rayHit.y >> 6, rayHit.x, rayHit.y);
			printedAngle = true;
		}
		*/
		// update renderer
		testWin.UpdateTexture();
	}

	return 0;
}