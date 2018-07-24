#include <stdio.h>
#include "Raycast.h"


int main(int argc, char** argv) {

	POINT wallIntersection;
	int angle;
	int mapArray[6][8] = {
							{ 0, 0, 0, 0, 0, 0, 0, 0 },
							{ 0, 1, 1, 1, 1, 1, 1, 0 },
							{ 0, 1, 1, 1, 1, 1, 1, 0 },
							{ 0, 1, 1, 1, 1, 1, 1, 0 },
							{ 0, 1, 1, 1, 1, 1, 1, 0 },
							{ 0, 0, 0, 0, 0, 0, 0, 0 }
						};
	POINT playerCoord = { 224, 360 };
	
	int wallDist;

	for (angle = 0; angle < 360; angle++) {

		wallIntersection = castRay(&playerCoord, angle, mapArray);
		wallDist = getDistanceToWall(&wallIntersection, &playerCoord);
		int wallHeight = (double)TILE_HEIGHT * (277 / (double)wallDist);
		if (wallDist > 64*8) {
			printf("OUT OF BOUNDS: walldist: %d - wallheight: %d - angle: %d \n", wallDist, wallHeight, angle);
		}
	}
	return 0;
}